#include <cstdint>
#include <cstdlib>

#include "benchmark/benchmark.h"

void BM_write_seq(benchmark::State& state) {
  const size_t size = state.range(0);
  volatile uint64_t* memory = static_cast<uint64_t*>(std::malloc(size));
  if (!memory) return;

  const int64_t fill = 8128;  // value to copy
  const size_t num_values = size / sizeof(fill);

  for (auto _ : state) {
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
  std::free((void*)memory);
  state.SetBytesProcessed(size * state.iterations());
  state.SetItemsProcessed((num_values)*state.iterations());
}

// run for 1kiB to 1GiB, increment in powers of 2
#define ARGS ->RangeMultiplier(2)->Range(1 << 10, 1 << 30)
BENCHMARK(BM_write_seq) ARGS;
BENCHMARK_MAIN();
