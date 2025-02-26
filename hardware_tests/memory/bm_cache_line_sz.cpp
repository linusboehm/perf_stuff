#include "benchmark/benchmark.h"

void BM_cache_line_sz(benchmark::State& state) {
  const int size = 32 << 20;
  const size_t stride = state.range(0);

  char* source = static_cast<char*>(std::malloc(size));
  char* dest = static_cast<char*>(std::malloc(size));
  for (auto _ : state) {
    for (int i = 0; i < size; i += stride) {
      benchmark::DoNotOptimize(dest[i] = source[i]);
    }
    benchmark::ClobberMemory();
  }
  std::free(source);
  std::free(dest);
}

#define ARGS ->DenseRange(16, 300, 1)
BENCHMARK(BM_cache_line_sz) ARGS;

BENCHMARK_MAIN();
