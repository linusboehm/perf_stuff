#include <sys/stat.h>
#include <unistd.h>

#include <iostream>
#include <memory>
#include <vector>

#include "include/loop_tester.hpp"

using PerfStuff::LoopTester::AbstractTestFunc;
using PerfStuff::LoopTester::RunParams;

struct TestIncrement : public AbstractTestFunc {
  TestIncrement() : AbstractTestFunc("incr") {}
  INLINE void run(RunParams& params) {
    cnt++;
  }
  ~TestIncrement() {
    std::cout << "nr tests in test func: " << cnt << std::endl;
  }
  int cnt = 0;
};

struct TestSleep : public AbstractTestFunc {
  TestSleep() : AbstractTestFunc("sleep") {}
  INLINE void run(RunParams& params) {
    usleep(100);
  }
};

int main() {
  std::vector<std::unique_ptr<AbstractTestFunc>> test_functions;
  test_functions.emplace_back(std::make_unique<TestIncrement>());
  test_functions.emplace_back(std::make_unique<TestSleep>());

  RunParams params{.bytes_ = 1000};

  for (auto& test_func : test_functions) {
    std::cout << "\n--- " << test_func->get_name() << " ---\n" << std::endl;
    test_func->measure(params);
  }
  return 0;
}
