#include <stdlib.h>

#include "benchmark/benchmark.h"

static constexpr uint32_t ITERATIONS = 100000;

void BM_branch_always(benchmark::State& state) {
  srand(1);
  std::vector<int> r(ITERATIONS);
  std::generate(r.begin(), r.end(), []() { return rand() > 0; });

  for (auto _ : state) {
    uint64_t res = 0;
    for (size_t i = 0; i < ITERATIONS; ++i) {
      if (r[i]) {
        res += 12345;
      } else {
        res *= 23456;
      }
    }
    benchmark::DoNotOptimize(res);
    benchmark::ClobberMemory();
  }
  state.SetItemsProcessed(ITERATIONS * state.iterations());
}

void BM_branch_random(benchmark::State& state) {
  srand(1);
  std::vector<int> r(ITERATIONS);
  std::generate(r.begin(), r.end(), []() { return rand() & 1; });

  for (auto _ : state) {
    uint64_t res = 0;
    for (size_t i = 0; i < ITERATIONS; ++i) {
      if (r[i]) {
        res += 12345;
      } else {
        res *= 23456;
      }
    }
    benchmark::DoNotOptimize(res);
    benchmark::ClobberMemory();
  }
  state.SetItemsProcessed(ITERATIONS * state.iterations());
}

BENCHMARK(BM_branch_always);
BENCHMARK(BM_branch_random);

BENCHMARK_MAIN();
