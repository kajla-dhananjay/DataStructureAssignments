#include "load_tester.hpp"

#include <fcntl.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <atomic>
#include <chrono>
#include <csignal>
#include <cstdint>
#include <cstring>
#include <future>
#include <iomanip>
#include <stdexcept>
#include <thread>

#include "common_utils.hpp"
#include "failure_logger.hpp"
#include "sorting_wrappers.hpp"
#include "testcase_generator.hpp"

using namespace std::chrono;

namespace {

bool write_all(int fd, const void* buffer, size_t count) {
  const char* buf = static_cast<const char*>(buffer);
  size_t total = 0;
  while (total < count) {
    ssize_t written = write(fd, buf + total, count - total);
    if (written <= 0) return false;
    total += written;
  }
  return true;
}

bool read_all(int fd, void* buffer, size_t count) {
  char* buf = static_cast<char*>(buffer);
  size_t total = 0;
  while (total < count) {
    ssize_t r = read(fd, buf + total, count - total);
    if (r <= 0) return false;
    total += r;
  }
  return true;
}

template <typename T>
struct VectorSerialization;

template <>
struct VectorSerialization<int> {
  static bool write(int fd, const std::vector<int>& vec) {
    uint64_t size = vec.size();
    return write_all(fd, &size, sizeof(size)) &&
           (size == 0 || write_all(fd, vec.data(), size * sizeof(int)));
  }

  static bool read(int fd, std::vector<int>& vec) {
    uint64_t size;
    if (!read_all(fd, &size, sizeof(size))) return false;
    vec.resize(size);
    return size == 0 || read_all(fd, vec.data(), size * sizeof(int));
  }
};

template <>
struct VectorSerialization<std::string> {
  static bool write(int fd, const std::vector<std::string>& vec) {
    uint64_t size = vec.size();
    if (!write_all(fd, &size, sizeof(size))) return false;
    for (const auto& s : vec) {
      uint64_t len = s.size();
      if (!write_all(fd, &len, sizeof(len)) || (len > 0 && !write_all(fd, s.data(), len)))
        return false;
    }
    return true;
  }

  static bool read(int fd, std::vector<std::string>& vec) {
    uint64_t size;
    if (!read_all(fd, &size, sizeof(size))) return false;
    vec.resize(size);
    for (auto& s : vec) {
      uint64_t len;
      if (!read_all(fd, &len, sizeof(len))) return false;
      s.resize(len);
      if (len > 0 && !read_all(fd, &s[0], len)) return false;
    }
    return true;
  }
};

}  // anonymous namespace

LoadTester::LoadTester(int timeout_seconds) : timeout_seconds_(timeout_seconds) {
  generate_test_sequence();
}

void LoadTester::generate_test_sequence() {
  const long max_n = 1e7;
  for (long n = 1; n <= max_n; n *= 2) {
    test_sizes_.push_back(n);
    if (n >= max_n) break;
  }
}

std::vector<LoadTester::RuntimeResult> LoadTester::run_load_tests() {
  std::vector<RuntimeResult> results;
  std::unordered_set<std::string> timed_out;

  DEBUG_LOG("Starting load tests with " << test_sizes_.size() << " sizes");

  for (long n : test_sizes_) {
    DEBUG_LOG("\n=== Testing N = " << n << " ===");

    auto process = [&](auto&& data, const std::string& type) {
      for (const auto& algo : algorithms_) {
        if (timed_out.count(algo + "_" + type)) {
          DEBUG_LOG("Skipping " << algo << " on " << type);
          continue;
        }
        results.push_back(test_algorithm(algo, data, type, timed_out));
      }
    };

    process(generate_testcase<int>(n), "int");
    process(generate_testcase<std::string>(n), "string");
  }

  DEBUG_LOG("\nLoad testing completed");
  return results;
}

template <typename T>
LoadTester::RuntimeResult LoadTester::test_algorithm(const std::string& algo_name,
                                                     const std::vector<T>& data,
                                                     const std::string& type,
                                                     std::unordered_set<std::string>& timed_out) {
  RuntimeResult result;
  result.algorithm = algo_name;
  result.n = data.size();
  result.data_type = type;

  DEBUG_LOG("Starting " << algo_name << " on " << type << " (N=" << result.n << ")");

  int pipefd[2];
  if (pipe(pipefd) == -1) throw std::runtime_error("pipe failed");

  pid_t pid = fork();
  if (pid < 0) throw std::runtime_error("fork failed");

  if (pid == 0) {
    close(pipefd[0]);

    sigset_t mask;
    sigfillset(&mask);
    sigprocmask(SIG_SETMASK, &mask, nullptr);

    try {
      auto start = high_resolution_clock::now();
      std::vector<T> sorted = (algo_name == "std_sort")
                                  ? sysSort(data)
                                  : userSort<T>(get_algorithm_id(algo_name), data);
      double runtime =
          duration_cast<duration<double>>(high_resolution_clock::now() - start).count();

      DEBUG_LOG_LABEL("Child", algo_name << " completed in " << std::fixed << std::setprecision(6)
                                         << runtime << "s");

      if (!write_all(pipefd[1], &runtime, sizeof(runtime)) ||
          !VectorSerialization<T>::write(pipefd[1], sorted)) {
        DEBUG_LOG_LABEL("Child", "IPC failure for " << algo_name);
        _exit(2);  // IPC error, this is not a user error, this means pipe is broken
      }
      DEBUG_LOG_LABEL("Child ", "Exiting " << algo_name);
      _exit(0);
    } catch (...) {
      _exit(3);  // Some random Exception
    }
  } else {
    close(pipefd[1]);

    struct timeval timeout = {.tv_sec = timeout_seconds_, .tv_usec = 0};
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(pipefd[0], &readfds);

    int ret = select(pipefd[0] + 1, &readfds, nullptr, nullptr, &timeout);

    if (ret == 0) {  // Timeout
      kill(pid, SIGKILL);
      waitpid(pid, nullptr,
              0);  // Reap zombie process. Required otherwise CPU hit 100% and process terminates
      result.runtime_seconds = -2.0;
      DEBUG_LOG("Adding " << algo_name << " on " << type << " to timed out set");
      timed_out.insert(algo_name + "_" + type);
      DEBUG_LOG("[Timeout] " << algo_name << " on " << type << " after " << timeout_seconds_
                             << "s");
      return result;
    }

    double child_runtime;
    std::vector<T> user_result;
    bool read_ok = read_all(pipefd[0], &child_runtime, sizeof(child_runtime)) &&
                   VectorSerialization<T>::read(pipefd[0], user_result);

    int status;
    waitpid(pid, &status, 0);

    if (WEXITSTATUS(status) != 0) {
      const int exit_code = WEXITSTATUS(status);
      result.runtime_seconds = (exit_code == 2) ? -4.0 : -3.0;
      DEBUG_LOG_LABEL("Error ", algo_name << " on " << type << " failed with code " << exit_code);
      return result;
    }

    if (!read_ok) {
      result.runtime_seconds = -4.0;
      DEBUG_LOG_LABEL("Error", "IPC failure for " << algo_name << " on " << type);
      return result;
    }

    auto sys_result = sysSort(data);
    const bool correct = (user_result == sys_result);
    if(!correct) {
      timed_out.insert(algo_name + "_" + type);
    }
    result.runtime_seconds = correct ? child_runtime : -1.0;

    DEBUG_LOG_LABEL("Result", algo_name << " on " << type << " | Runtime: " << std::fixed
                                        << std::setprecision(6) << child_runtime
                                        << "s | Correct: " << (correct ? "Yes" : "No"));

    if (!correct) {
      DEBUG_LOG_LABEL("Mismatch", "Logging failure details for " << algo_name);
      log_failure<T>(algo_name, type, data.size(), data, sys_result, user_result);
    }
  }

  return result;
}

int LoadTester::get_algorithm_id(const std::string& algo_name) {
  static const std::unordered_map<std::string, int> algo_ids = {
      {"bubble_sort", 0}, {"insertion_sort", 1}, {"merge_sort", 2}, {"quick_sort", 3}};
  auto it = algo_ids.find(algo_name);
  if (it == algo_ids.end()) throw std::invalid_argument("Unknown algorithm");
  return it->second;
}

template LoadTester::RuntimeResult LoadTester::test_algorithm<int>(
    const std::string&, const std::vector<int>&, const std::string&,
    std::unordered_set<std::string>&);

template LoadTester::RuntimeResult LoadTester::test_algorithm<std::string>(
    const std::string&, const std::vector<std::string>&, const std::string&,
    std::unordered_set<std::string>&);