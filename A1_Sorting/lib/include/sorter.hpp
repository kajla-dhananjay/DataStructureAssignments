#ifndef SORTER_HPP
#define SORTER_HPP

#include <string>
#include <vector>

template <typename T>
class sorter {
 public:
  virtual ~sorter() = default;
  virtual std::vector<T> sort(const std::vector<T>& input) = 0;
};

template <typename T>
class bubble_sort : public sorter<T> {
 public:
  std::vector<T> sort(const std::vector<T>& input) override;
};

template <typename T>
class insertion_sort : public sorter<T> {
 public:
  std::vector<T> sort(const std::vector<T>& input) override;
};

template <typename T>
class merge_sort : public sorter<T> {
 public:
  std::vector<T> sort(const std::vector<T>& input) override;
};

template <typename T>
class quick_sort : public sorter<T> {
 public:
  std::vector<T> sort(const std::vector<T>& input) override;
};

#endif  // SORTER_HPP
