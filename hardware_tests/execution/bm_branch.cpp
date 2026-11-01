#include <stdlib.h>
#include <random>

#include "benchmark/benchmark.h"


long function1(long arg1, long arg2, int *p1, int *p2) {
    return arg1 ^ arg2 ^ (p1 - p2);
}

long function2(long arg1, long arg2, int *p1, int *p2) {
    return p1 - p2 + arg1 - arg2;
}

static constexpr uint32_t ITERATIONS = 11;
static constexpr uint32_t VECTOR_LENGTH = 1021;

void BM_indirect(benchmark::State& state) {
  const size_t shift = state.range(0);
  std::mt19937 mt_rand;
  using Signature = long(long, long, int *, int *);
  auto denominator = 1 << shift;
  std::vector<Signature*> r(denominator, function1);
  r.insert(r.end(), VECTOR_LENGTH-denominator, function2);
  std::shuffle(r.begin(), r.end(), mt_rand);

  long l1 = 0, l2 = 0;
  int i1 = 0, i2 = 0, *p1 = &i1, *p2 = &i2;

  auto result = 0;
  for (auto _ : state) {
    for (size_t i = 0; i < ITERATIONS; ++i) {
      for(auto target: r) { result ^= target(l1, l2, p1, p2); }
    }
    benchmark::DoNotOptimize(result);
  }
  // state.SetItemsProcessed(ITERATIONS * state.iterations());
}

// void BM_branch_random(benchmark::State& state) {
//   std::vector<int> r(ITERATIONS/2, 1);
//   r.insert(r.end(), ITERATIONS/2, 0);
//   std::shuffle(r.begin(), r.end(), mt_rand);
//
//   for (auto _ : state) {
//     uint64_t res = 0;
//     for (size_t i = 0; i < ITERATIONS; ++i) {
//       if (r[i]) {
//         res *= 23456;
//       } else {
//         res += 12345;
//       }
//     }
//     benchmark::DoNotOptimize(res);
//     benchmark::ClobberMemory();
//   }
//   state.SetItemsProcessed(ITERATIONS * state.iterations());
// }

// #define ARGS ->DenseRange(1, VECTOR_LENGTH, 1)
#define ARGS ->DenseRange(1, 9)
BENCHMARK(BM_indirect) ARGS;
// BENCHMARK(BM_branch_random);
// #define ARGS ->RangeMultiplier(2)->Range(1 << 10, 1 << 30)
// BENCHMARK(BM_write_seq) ARGS;

BENCHMARK_MAIN();
