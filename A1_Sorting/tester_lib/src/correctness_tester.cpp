#include "correctness_tester.hpp"

#include <chrono>
#include <future>
#include <iostream>
#include <stdexcept>
#include <typeinfo>
#include <vector>

#include "common_utils.hpp"
#include "failure_logger.hpp"
#include "sorting_wrappers.hpp"
#include "testcase_generator.hpp"

CorrectnessTester::CorrectnessTester(Config config) : config_(config) {}

void CorrectnessTester::run_all_tests() {
  run_type_tests<int>();
  run_type_tests<std::string>();
}

template <typename T>
void CorrectnessTester::run_type_tests() {
  const char* algo_names[] = {"bubble_sort", "insertion_sort", "merge_sort", "quick_sort"};
  const std::string type_name = TypeName<T>::name();

  for (int algo = 0; algo < 4; ++algo) {
    const std::string algo_name = algo_names[algo];

    for (int i = 0; i < config_.num_sanity_tests; ++i) {
      current_test_++;
      DEBUG_LOG("Running SANITY test " << current_test_ << "/200 (" << algo_name << " on "
                                       << type_name << ")");

      try {
        std::vector<T> data = generate_testcase<T>(config_.sanity_test_size);
        std::future<std::vector<T>> future =
            std::async(std::launch::async, [&] { return userSort<T>(algo, data); });

        if (future.wait_for(std::chrono::seconds(config_.timeout_seconds)) !=
            std::future_status::ready) {
          DEBUG_LOG("  ↳ Timeout after " << config_.timeout_seconds << "s");
          summary_[algo_name][type_name].fails++;
          continue;
        }

        std::vector<T> user_result = future.get();
        std::vector<T> sys_result = sysSort<T>(data);

        if (user_result == sys_result) {
          summary_[algo_name][type_name].passes++;
          score_++;
          DEBUG_LOG("  ↳ Passed");
        } else {
          summary_[algo_name][type_name].fails++;
          DEBUG_LOG("  ↳ Failed");
          log_failure<T>(algo_name, type_name, config_.sanity_test_size, data, sys_result,
                         user_result);
        }
      } catch (...) {
        summary_[algo_name][type_name].fails++;
        DEBUG_LOG("  ↳ Exception occurred");
      }
    }

    const int standard_size = (algo < 2) ? config_.test_size_slow : config_.test_size_fast;

    for (int i = 0; i < config_.num_standard_tests; ++i) {
      current_test_++;
      DEBUG_LOG("Running STANDARD test " << current_test_ << "/200 (" << algo_name << " on "
                                         << type_name << ")");

      try {
        std::vector<T> data = generate_testcase<T>(standard_size);
        std::future<std::vector<T>> future =
            std::async(std::launch::async, [&] { return userSort<T>(algo, data); });

        if (future.wait_for(std::chrono::seconds(config_.timeout_seconds)) !=
            std::future_status::ready) {
          DEBUG_LOG("  ↳ Timeout after " << config_.timeout_seconds << "s");
          summary_[algo_name][type_name].fails++;
          continue;
        }

        std::vector<T> user_result = future.get();
        std::vector<T> sys_result = sysSort<T>(data);

        if (user_result == sys_result) {
          summary_[algo_name][type_name].passes++;
          score_++;
          DEBUG_LOG("  ↳ Passed");
        } else {
          summary_[algo_name][type_name].fails++;
          DEBUG_LOG("  ↳ Failed");
          log_failure<T>(algo_name, type_name, standard_size, data, sys_result, user_result);
        }
      } catch (...) {
        summary_[algo_name][type_name].fails++;
        DEBUG_LOG("  ↳ Exception occurred");
      }
    }
  }
}

void CorrectnessTester::print_summary() const {
  std::cout << "{\n";
  bool first_algo = true;
  for (const auto& [algo, counts] : summary_) {
    if (!first_algo) std::cout << ",\n";
    first_algo = false;

    std::cout << "  \"" << algo << "\": {\n"
              << "    \"int\": {\"pass\": " << counts.at("int").passes
              << ", \"fail\": " << counts.at("int").fails << "},\n"
              << "    \"string\": {\"pass\": " << counts.at("string").passes
              << ", \"fail\": " << counts.at("string").fails << "}\n"
              << "  }";
  }
  std::cout << "\n}\n";
}

template void CorrectnessTester::run_type_tests<int>();
template void CorrectnessTester::run_type_tests<std::string>();

int CorrectnessTester::get_score() const { return score_; }