# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

set(ALL_NASM_BM_TARGETS asm_bm_frontend asm_bm_dep_chains asm_bm_ilp)
message("GoogleBenchmark targets to be built: ${ALL_NASM_BM_TARGETS}")

foreach(target ${ALL_NASM_BM_TARGETS})
  add_executable(perf_stuff.${target})
  target_sources(perf_stuff.${target} PRIVATE ${target}.cpp ${target}.asm)
  target_link_libraries(perf_stuff.${target} PRIVATE benchmark::benchmark)
endforeach()

set(ALL_BM_TARGETS bm_branch)
message("GoogleBenchmark targets to be built: ${ALL_BM_TARGETS}")

foreach(target ${ALL_BM_TARGETS})
  add_executable(perf_stuff.${target})
  target_sources(perf_stuff.${target} PRIVATE ${target}.cpp)
  target_link_libraries(perf_stuff.${target} PRIVATE benchmark::benchmark)
endforeach()
