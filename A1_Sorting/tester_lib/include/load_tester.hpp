#pragma once
#include <chrono>
#include <future>
#include <string>
#include <unordered_set>
#include <vector>

class LoadTester {
 public:
  struct RuntimeResult {
    std::string algorithm;
    std::string data_type;
    long n;
    double runtime_seconds;
  };

  explicit LoadTester(int timeout_seconds = 30);
  std::vector<RuntimeResult> run_load_tests();

 private:
  int timeout_seconds_;
  std::vector<long> test_sizes_;
  std::vector<std::string> algorithms_ = {"bubble_sort", "insertion_sort", "merge_sort",
                                          "quick_sort", "std_sort"};

  void generate_test_sequence();

  template <typename T>
  RuntimeResult test_algorithm(const std::string& algo_name, const std::vector<T>& data,
                               const std::string& type, std::unordered_set<std::string>& timed_out);

  template <typename T>
  bool should_skip(const std::string& algo, long n, const std::string& type,
                   std::unordered_set<std::string>& timed_out);

  template <typename T>
  RuntimeResult test_algorithm_wrapper(const std::string& algo_name, const std::vector<T>& data,
                                       std::unordered_set<std::string>& timed_out);

  int get_algorithm_id(const std::string& algo_name);
};