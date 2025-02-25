#pragma once
#include <algorithm>
#include <vector>

#include "sorter.hpp"

template <typename T>
std::vector<T> userSort(int sortType, const std::vector<T>& arr) {
  switch (sortType) {
    case 0:
      return bubble_sort<T>().sort(arr);
    case 1:
      return insertion_sort<T>().sort(arr);
    case 2:
      return merge_sort<T>().sort(arr);
    case 3:
      return quick_sort<T>().sort(arr);
    default:
      throw std::invalid_argument("Invalid sort type");
  }
}

template <typename T>
std::vector<T> sysSort(const std::vector<T>& arr) {
  std::vector<T> copy(arr);
  std::sort(copy.begin(), copy.end());
  return copy;
}