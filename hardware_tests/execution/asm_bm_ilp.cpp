#include <sys/mman.h>

#include <cstdint>

#include "benchmark/benchmark.h"

extern "C" int64_t ASM_mov1(uint64_t Count, uint64_t* Data);
extern "C" int64_t ASM_mov2(uint64_t Count, uint64_t* Data);
extern "C" int64_t ASM_mov3(uint64_t Count, uint64_t* Data);
extern "C" int64_t ASM_mov_add(uint64_t Count, uint64_t* Data);

char* do_mmap(const size_t size) {
  return static_cast<char*>(
      mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
}

void BM_mov1(benchmark::State& state) {
  uint64_t num = 0;
  uint64_t val = 123;
  for (auto _ : state) {
    num = ASM_mov1(1000000, &val);
    benchmark::DoNotOptimize(num);
    benchmark::ClobberMemory();
  }
}

void BM_mov2(benchmark::State& state) {
  uint64_t num = 0;
  uint64_t val = 123;
  for (auto _ : state) {
    num = ASM_mov2(1000000, &val);
    benchmark::DoNotOptimize(num);
    benchmark::ClobberMemory();
  }
}

void BM_mov3(benchmark::State& state) {
  uint64_t num = 0;
  uint64_t val = 123;
  for (auto _ : state) {
    num = ASM_mov3(1000000, &val);
    benchmark::DoNotOptimize(num);
    benchmark::ClobberMemory();
  }
}

void BM_mov_add(benchmark::State& state) {
  uint64_t num = 0;
  uint64_t val = 123;
  for (auto _ : state) {
    num = ASM_mov_add(1000000, &val);
    benchmark::DoNotOptimize(num);
    benchmark::ClobberMemory();
  }
}

BENCHMARK(BM_mov1);
BENCHMARK(BM_mov2);
BENCHMARK(BM_mov3);
BENCHMARK(BM_mov_add);

BENCHMARK_MAIN();
