#pragma once
#include <iostream>
#include <string>
#include <vector>

std::vector<int> generate_int_testcase(int N);
std::vector<std::string> generate_string_testcase(int N);

template <typename T>
std::vector<T> generate_testcase(int N) {
  if constexpr (std::is_same_v<T, int>) {
    return generate_int_testcase(N);
  } else {
    return generate_string_testcase(N);
  }
}