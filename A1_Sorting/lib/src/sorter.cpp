#include "sorter.hpp"

/********************************************************
 * User code starts here
 ********************************************************/

template <typename T>
std::vector<T> bubble_sort<T>::sort(const std::vector<T>& input) {
  std::vector<T> result;
  /***
   * Your code here
   */
  return result;
}

template <typename T>
std::vector<T> insertion_sort<T>::sort(const std::vector<T>& input) {
  std::vector<T> result;
  /***
   * Your code here
   */
  return result;
}

template <typename T>
std::vector<T> merge_sort<T>::sort(const std::vector<T>& input) {
  std::vector<T> result;
  /***
   * Your code here
   */
  return result;
}

template <typename T>
std::vector<T> quick_sort<T>::sort(const std::vector<T>& input) {
  std::vector<T> result;
  /***
   * Your code here
   */
  return result;
}

// Explicit template instantiation
template class bubble_sort<int>;
template class insertion_sort<int>;
template class merge_sort<int>;
template class quick_sort<int>;

template class bubble_sort<std::string>;
template class insertion_sort<std::string>;
template class merge_sort<std::string>;
template class quick_sort<std::string>;