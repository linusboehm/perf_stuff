#include <cstdint>
#include <cstdlib>

#include "benchmark/benchmark.h"

// measure misses with:
// perf stat -e cache-misses,L1-dcache-load-misses ./<binary>

void BM_write_seq(benchmark::State& state) {
  const size_t size = 16<<10;
  // const size_t size = 128<<10;
  // const size_t size = 65536<<10;
  const size_t iterations = (1<<30) / size;
  volatile uint64_t* memory = static_cast<uint64_t*>(std::malloc(size));
  if (!memory)
    return;

  int64_t fill = 12345;  // value to copy
  const size_t num_values = size / sizeof(fill);

  for (auto _ : state) {
    for (int64_t j = 0; j < iterations; ++j) {
    for (int64_t i = 0; i < num_values;) {
      *(memory + i++) = fill;
      *(memory + i++) = fill;
      *(memory + i++) = fill;
      *(memory + i++) = fill;
      *(memory + i++) = fill;
      *(memory + i++) = fill;
      *(memory + i++) = fill;
      *(memory + i++) = fill;
      *(memory + i++) = fill;
      *(memory + i++) = fill;
      *(memory + i++) = fill;
      *(memory + i++) = fill;
      *(memory + i++) = fill;
      *(memory + i++) = fill;
      *(memory + i++) = fill;
      *(memory + i++) = fill;
      *(memory + i++) = fill;
      *(memory + i++) = fill;
      *(memory + i++) = fill;
      *(memory + i++) = fill;
      *(memory + i++) = fill;
      *(memory + i++) = fill;
      *(memory + i++) = fill;
      *(memory + i++) = fill;
      *(memory + i++) = fill;
      *(memory + i++) = fill;
      *(memory + i++) = fill;
      *(memory + i++) = fill;
      *(memory + i++) = fill;
      *(memory + i++) = fill;
      *(memory + i++) = fill;
      *(memory + i++) = fill;
    }
    }
  }
  std::free((void*)memory);
  state.SetBytesProcessed((1<<30)/size * state.iterations());
  state.SetItemsProcessed((num_values)*state.iterations());
}

BENCHMARK(BM_write_seq)->Iterations(10);
BENCHMARK_MAIN();
