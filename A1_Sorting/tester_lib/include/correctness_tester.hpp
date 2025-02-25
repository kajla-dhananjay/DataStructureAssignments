#pragma once
#include <chrono>
#include <future>
#include <map>
#include <string>
#include <vector>

class CorrectnessTester {
 public:
  struct Config {
    int num_sanity_tests;
    int num_standard_tests;
    int sanity_test_size;
    int test_size_slow;
    int test_size_medium;
    int test_size_fast;
    int timeout_seconds;

    Config()
        : num_sanity_tests(5),
          num_standard_tests(20),
          sanity_test_size(14),
          test_size_slow(2'000),
          test_size_medium(5'000),
          test_size_fast(100'000),
          timeout_seconds(5) {}
  };

  struct TestCounts {
    int passes = 0;
    int fails = 0;
  };

  explicit CorrectnessTester(Config config = Config{});
  void run_all_tests();
  void print_summary() const;
  int get_score() const;

 private:
  Config config_;
  int current_test_ = 0;
  int score_ = 0;
  std::map<std::string, std::map<std::string, TestCounts>> summary_;

  template <typename T>
  void run_type_tests();
};