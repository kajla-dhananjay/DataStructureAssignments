#include "testcase_generator.hpp"

#include <algorithm>
#include <random>
#include <string>

std::vector<int> generate_int_testcase(int N) {
  // DEBUG_LOG("Generating " << N << " random integers");
  std::vector<int> arr(N);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(-1e9, 1e9);

  std::generate(arr.begin(), arr.end(), [&]() { return dist(gen); });
  // DEBUG_LOG("Generated " << N << " random integers");
  return arr;
}

std::vector<std::string> generate_string_testcase(int N) {
  // DEBUG_LOG("Generating " << N << " random strings");
  std::vector<std::string> arr(N);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> len_dist(1, 10);
  std::uniform_int_distribution<int> char_dist('a', 'z');

  for (auto& s : arr) {
    s.resize(len_dist(gen));
    std::generate(s.begin(), s.end(), [&]() { return static_cast<char>(char_dist(gen)); });
  }
  // DEBUG_LOG("Generated " << N << " random strings");
  return arr;
}