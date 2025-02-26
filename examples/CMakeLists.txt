# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

set(ALL_TARGETS profiler_example profiler_example_modulo loop_tester_example)

message("Targets to be built: ${ALL_TARGETS}")

foreach(target ${ALL_TARGETS})
  add_executable(perf_stuff.${target})
  target_sources(perf_stuff.${target} PRIVATE ${target}.cpp)

  target_link_libraries(perf_stuff.${target} PRIVATE perf::stuff)
endforeach()

set(ALL_ASM_TARGETS nasm_add)

message("ASM Targets to be built: ${ALL_ASM_TARGETS}")

foreach(target ${ALL_ASM_TARGETS})
  add_executable(perf_stuff.${target})
  target_sources(perf_stuff.${target} PRIVATE ${target}.cpp ${target}.asm)
endforeach()

set(ALL_BM_TARGETS bm_modulo bm_sorted)
message("GoogleBenchmark targets to be built: ${ALL_BM_TARGETS}")

foreach(target ${ALL_BM_TARGETS})
  add_executable(perf_stuff.${target})
  target_sources(perf_stuff.${target} PRIVATE ${target}.cpp)
  target_link_libraries(perf_stuff.${target} PRIVATE benchmark::benchmark)
endforeach()
