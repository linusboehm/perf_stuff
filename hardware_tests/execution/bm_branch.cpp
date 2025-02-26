#include <stdlib.h>
#include <random>

#include "benchmark/benchmark.h"

static constexpr uint32_t ITERATIONS = 100000;

void BM_branch_always(benchmark::State& state) {
  std::vector<int> r(ITERATIONS/2, 1);
  r.insert(r.end(), ITERATIONS/2, 0);

  for (auto _ : state) {
    uint64_t res = 0;
    for (size_t i = 0; i < ITERATIONS; ++i) {
      if (r[i]) {
        res *= 23456;
      } else {
        res += 12345;
      }
    }
    benchmark::DoNotOptimize(res);
    benchmark::ClobberMemory();
  }
  state.SetItemsProcessed(ITERATIONS * state.iterations());
}

void BM_branch_random(benchmark::State& state) {
  std::mt19937 mt_rand;
  std::vector<int> r(ITERATIONS/2, 1);
  r.insert(r.end(), ITERATIONS/2, 0);
  std::shuffle(r.begin(), r.end(), mt_rand);

  for (auto _ : state) {
    uint64_t res = 0;
    for (size_t i = 0; i < ITERATIONS; ++i) {
      if (r[i]) {
        res *= 23456;
      } else {
        res += 12345;
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
