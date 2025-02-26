#include <cstdint>

#include "benchmark/benchmark.h"

extern "C" int64_t ASM_add(int64_t start_val);
extern "C" int64_t ASM_add_loop(int64_t iterations);
extern "C" void ASM_nop_short();
extern "C" void ASM_nop_short_loop(int64_t iterations);
extern "C" void ASM_nop_long();
extern "C" void ASM_nop_long_loop(int64_t iterations);

void BM_add(benchmark::State& state) {
  int64_t num = 0;
  for (auto _ : state) {
    num = ASM_add(0);
    benchmark::DoNotOptimize(num);
    benchmark::ClobberMemory();
  }
}

void BM_add_loop(benchmark::State& state) {
  int64_t num = 0;
  for (auto _ : state) {
    num = ASM_add_loop(1000000);
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
    ASM_nop_short_loop(1000000);
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
    ASM_nop_long_loop(1000000);
    benchmark::DoNotOptimize(num);
    benchmark::ClobberMemory();
  }
}

// mention that this can be used to measure different instruction latencies (division...)
// /////////////////////////////////
// BM_add: 324759 ns
//         8,019,916,535      instructions              #    1.00  insn per cycle
//           167,719,149      idq_uops_not_delivered.core
//             2,650,069      icache.misses
//         8,033,305,538      uops_issued.any             -> all instructions were translated into a single uop
//         8,033,569,822      uops_executed.core          -> all uops were executed
//         5,993,059,127      resource_stalls.any         -> limited by backend, lots of backend stalls
//
// BM_add_loop: 323744 ns
//        24.011.861.527      instructions              #    2.99  insn per cycle
//            95.244.063      idq_uops_not_delivered.core
//                60.204      icache.misses
//        16.016.884.513      uops_issued.any
//        16.016.475.053      uops_executed.core
//         3.996.397.072      resource_stalls.any
//
// /////////////////////////////////
// BM_nop_short: 81500 ns
//         8,011,622,500      instructions              #    3.95  insn per cycle -> frontend can decode four instructions at a time
//            70,003,648      idq_uops_not_delivered.core
//               807,231      icache.misses
//         8,016,835,160      uops_issued.any             -> 8,000,000,000 nops issued
//            16,775,989      uops_executed.core          -> backend doesn't execute the noops
//             1,566,922      resource_stalls.any
//
// /////////////////////////////////
// BM_nop_short_loop: 324464 ns                           -> same speed as add, limited by backend `dec` dependency chain
//        24,018,463,950      instructions              #    2.99  insn per cycle
//           142,150,728      idq_uops_not_delivered.core
//                70,465      icache.misses
//        16,028,027,105      uops_issued.any          -> less than instructions -> dec and jnz get fused into micro op
//         8,027,586,815      uops_executed.core
//         3,992,141,482      resource_stalls.any
//
// /////////////////////////////////
// BM_nop_long: 459925 ns
//         8,025,860,069      instructions              #    0.70  insn per cycle
//        37,489,631,168      idq_uops_not_delivered.core -> front end did not deliver uops -> front end stalls -> couldn't fetch/decode instructions fast enough
//           833,700,996      icache.misses               -> instructions were not in instruction cache
//         8,047,736,012      uops_issued.any
//            48,413,489      uops_executed.core
//             3,214,621      resource_stalls.any
//
// /////////////////////////////////
// BM_nop_long_loop: 324018 ns
//        24,018,251,303      instructions              #    2.99  insn per cycle
//            56,707,042      idq_uops_not_delivered.core -> these must have been cached
//                64,015      icache.misses
//        16,027,721,673      uops_issued.any
//         8,027,260,545      uops_executed.core
//         4,002,854,455      resource_stalls.any

BENCHMARK(BM_add)->Iterations(8000);
BENCHMARK(BM_add_loop)->Iterations(8000);
BENCHMARK(BM_nop_short)->Iterations(8000);
BENCHMARK(BM_nop_short_loop)->Iterations(8000);
BENCHMARK(BM_nop_long)->Iterations(8000);
BENCHMARK(BM_nop_long_loop)->Iterations(8000);

BENCHMARK_MAIN();
