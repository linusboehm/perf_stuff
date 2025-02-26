#include <cstdint>
#include <cstdlib>
#include <numeric>
#include <vector>

#include "benchmark/benchmark.h"

void BM_write_rand(benchmark::State& state) {
  const size_t size = state.range(0);
  volatile uint64_t* memory = static_cast<uint64_t*>(std::malloc(size));
  if (!memory)
    return;

  int64_t fill = 12345;  // value to copy
  const size_t num_values = size / sizeof(fill);
  std::vector<int64_t> indexes(num_values);
  std::iota(indexes.begin(), indexes.end(), 0);
  std::random_shuffle(indexes.begin(), indexes.end());

  for (auto _ : state) {
    for (int64_t i = 0; i < num_values;) {
      *(memory + indexes[i++]) = fill;
      *(memory + indexes[i++]) = fill;
      *(memory + indexes[i++]) = fill;
      *(memory + indexes[i++]) = fill;
      *(memory + indexes[i++]) = fill;
      *(memory + indexes[i++]) = fill;
      *(memory + indexes[i++]) = fill;
      *(memory + indexes[i++]) = fill;
      *(memory + indexes[i++]) = fill;
      *(memory + indexes[i++]) = fill;
      *(memory + indexes[i++]) = fill;
      *(memory + indexes[i++]) = fill;
      *(memory + indexes[i++]) = fill;
      *(memory + indexes[i++]) = fill;
      *(memory + indexes[i++]) = fill;
      *(memory + indexes[i++]) = fill;
      *(memory + indexes[i++]) = fill;
      *(memory + indexes[i++]) = fill;
      *(memory + indexes[i++]) = fill;
      *(memory + indexes[i++]) = fill;
      *(memory + indexes[i++]) = fill;
      *(memory + indexes[i++]) = fill;
      *(memory + indexes[i++]) = fill;
      *(memory + indexes[i++]) = fill;
      *(memory + indexes[i++]) = fill;
      *(memory + indexes[i++]) = fill;
      *(memory + indexes[i++]) = fill;
      *(memory + indexes[i++]) = fill;
      *(memory + indexes[i++]) = fill;
      *(memory + indexes[i++]) = fill;
      *(memory + indexes[i++]) = fill;
      *(memory + indexes[i++]) = fill;
    }
  }
  std::free((void*)memory);
  state.SetBytesProcessed(size * state.iterations());
  state.SetItemsProcessed((num_values)*state.iterations());
}

// run for 1kiB to 1GiB
#define ARGS ->RangeMultiplier(2)->Range(1 << 10, 1 << 30)
BENCHMARK(BM_write_rand) ARGS;
BENCHMARK_MAIN();
