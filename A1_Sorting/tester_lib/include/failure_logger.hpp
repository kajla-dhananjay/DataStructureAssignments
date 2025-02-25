#include <fstream>
#include <mutex>
#include <unordered_set>

#include "common_utils.hpp"

namespace {
std::mutex log_mutex;
std::unordered_set<std::string> logged_failures;
}  // namespace

template <typename T>
void log_failure(const std::string& algorithm, const std::string& data_type, long n,
                 const std::vector<T>& input, const std::vector<T>& expected,
                 const std::vector<T>& actual) {
  std::lock_guard<std::mutex> lock(log_mutex);

  std::string failure_key = algorithm + "_" + data_type;

  if (logged_failures.find(failure_key) != logged_failures.end()) {
    return;
  }

  logged_failures.insert(failure_key);

  std::ofstream logfile("test_failures.log", std::ios::trunc);
  if (!logfile.is_open()) return;

  logfile << "=== TEST FAILURE ===\n";
  logfile << "Failure Key: " << failure_key << "\n";
  logfile << "Algorithm: " << algorithm << "\n";
  logfile << "Data Type: " << data_type << "\n";
  logfile << "Input Size: " << n << "\n";

  logfile << "Input Data: [";
  for (size_t i = 0; i < input.size(); ++i) {
    if (i > 0) logfile << " ";
    logfile << input[i];
  }
  logfile << "]\n";

  logfile << "Expected Output: [";
  for (size_t i = 0; i < expected.size(); ++i) {
    if (i > 0) logfile << " ";
    logfile << expected[i];
  }
  logfile << "]\n";

  logfile << "Actual Output: [";
  for (size_t i = 0; i < actual.size(); ++i) {
    if (i > 0) logfile << " ";
    logfile << actual[i];
  }
  logfile << "]\n\n";

  logfile.close();
}

template void log_failure<int>(const std::string&, const std::string&, long,
                               const std::vector<int>&, const std::vector<int>&,
                               const std::vector<int>&);

template void log_failure<std::string>(const std::string&, const std::string&, long,
                                       const std::vector<std::string>&,
                                       const std::vector<std::string>&,
                                       const std::vector<std::string>&);