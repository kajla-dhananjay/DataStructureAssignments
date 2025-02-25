#include <csignal>
#include <iostream>

#include "correctness_tester.hpp"
#include "load_tester.hpp"

void handle_sigusr1(int) {}
int main() {
  signal(SIGUSR1, handle_sigusr1);
  CorrectnessTester correctness_tester;
  correctness_tester.run_all_tests();

  std::cerr << "\nCorrectness Score: " << correctness_tester.get_score() << "/200\n";
  correctness_tester.print_summary();

  LoadTester load_tester;
  auto results = load_tester.run_load_tests();

  std::cout << "\nPerformance Results:\n";
  for (const auto& res : results) {
    std::cout << res.algorithm << "," << res.data_type << "," << res.n << "," << res.runtime_seconds
              << "\n";
  }

  return 0;
}