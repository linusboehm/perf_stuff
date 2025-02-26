#include <cstdint>

#include "benchmark/benchmark.h"

extern "C" int64_t ASM_add();
extern "C" int64_t ASM_add_loop(int64_t iterations);
extern "C" int64_t ASM_add_loop_unrolled();
extern "C" void ASM_nop_short();
extern "C" void ASM_nop_short_loop(int64_t iterations);
extern "C" void ASM_nop_long();
extern "C" void ASM_nop_long_loop(int64_t iterations);

void BM_add(benchmark::State& state) {
  int64_t num = 0;
  for (auto _ : state) {
    num = ASM_add();
    benchmark::DoNotOptimize(num);
    benchmark::ClobberMemory();
  }
}

void BM_add_loop(benchmark::State& state) {
  int64_t num = 0;
  for (auto _ : state) {
    num = ASM_add_loop(100'000);
    benchmark::DoNotOptimize(num);
    benchmark::ClobberMemory();
  }
}

void BM_add_loop_unrolled(benchmark::State& state) {
  int64_t num = 0;
  for (auto _ : state) {
    num = ASM_add_loop_unrolled();
    benchmark::DoNotOptimize(num);
    benchmark::ClobberMemory();
  }
}


void BM_nop_short(benchmark::State& state) {
  int64_t num = 0;
  for (auto _ : state) {
    ASM_nop_short();
    benchmark::DoNotOptimize(num);
    benchmark::ClobberMemory();
  }
}

void BM_nop_short_loop(benchmark::State& state) {
  int64_t num = 0;
  for (auto _ : state) {
    ASM_nop_short_loop(100'000);
    benchmark::DoNotOptimize(num);
    benchmark::ClobberMemory();
  }
}

void BM_nop_long(benchmark::State& state) {
  int64_t num = 0;
  for (auto _ : state) {
    ASM_nop_long();
    benchmark::DoNotOptimize(num);
    benchmark::ClobberMemory();
  }
}

void BM_nop_long_loop(benchmark::State& state) {
  int64_t num = 0;
  for (auto _ : state) {
    ASM_nop_long_loop(100'000);
    benchmark::DoNotOptimize(num);
    benchmark::ClobberMemory();
  }
}

BENCHMARK(BM_add)->Iterations(8000);
BENCHMARK(BM_add_loop)->Iterations(8000);
BENCHMARK(BM_nop_short)->Iterations(8000);
BENCHMARK(BM_nop_short_loop)->Iterations(8000);
BENCHMARK(BM_add_loop_unrolled)->Iterations(8000);
BENCHMARK(BM_nop_long)->Iterations(8000);
BENCHMARK(BM_nop_long_loop)->Iterations(8000);

BENCHMARK_MAIN();
