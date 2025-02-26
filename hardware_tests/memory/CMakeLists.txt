# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

set(ALL_TARGETS page_faults)

message("Targets to be built: ${ALL_TARGETS}")

foreach(target ${ALL_TARGETS})
  add_executable(perf_stuff.${target})
  target_sources(perf_stuff.${target} PRIVATE ${target}.cpp)

  target_link_libraries(perf_stuff.${target} PRIVATE perf::stuff)
endforeach()

set(ALL_BM_TARGETS bm_cache_sequ_write bm_cache_random_write bm_cache_line_sz)
message("GoogleBenchmark targets to be built: ${ALL_BM_TARGETS}")

foreach(target ${ALL_BM_TARGETS})
  add_executable(perf_stuff.${target})
  target_sources(perf_stuff.${target} PRIVATE ${target}.cpp)
  target_link_libraries(perf_stuff.${target} PRIVATE benchmark::benchmark)
endforeach()
