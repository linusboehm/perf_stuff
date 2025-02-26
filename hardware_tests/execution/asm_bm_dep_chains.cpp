#include <cstdint>

#include "benchmark/benchmark.h"

extern "C" int64_t ASM_add();
extern "C" int64_t ASM_mov_add();

void BM_add(benchmark::State& state) {
  int64_t num = 0;
  for (auto _ : state) {
    num = ASM_add();
    benchmark::DoNotOptimize(num);
    benchmark::ClobberMemory();
  }
}

void BM_mov_add(benchmark::State& state) {
  int64_t num = 0;
  int64_t mul_res = 9;
  for (auto _ : state) {
    num = ASM_mov_add();
    benchmark::DoNotOptimize(num);
    benchmark::ClobberMemory();
  }
}

BENCHMARK(BM_add);
BENCHMARK(BM_mov_add);

BENCHMARK_MAIN();
