#include "benchmark/benchmark.h"

void BM_modulo(benchmark::State& state) {
  uint64_t num = 0;
  for (auto _ : state) {
    num = ++num % 7;
  }
  benchmark::DoNotOptimize(num);
}

BENCHMARK(BM_modulo);
BENCHMARK(BM_modulo);
BENCHMARK(BM_modulo);
BENCHMARK(BM_modulo);
BENCHMARK(BM_modulo);
BENCHMARK(BM_modulo);
BENCHMARK(BM_modulo);
BENCHMARK_MAIN();
