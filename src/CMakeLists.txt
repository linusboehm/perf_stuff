# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

add_library(perf_stuff)
add_library(perf::stuff ALIAS perf_stuff)

target_sources(perf_stuff PRIVATE profiler.cpp linux_stuff.cpp loop_tester.cpp)

target_sources(
  perf_stuff
  PUBLIC FILE_SET
         HEADERS
         BASE_DIRS
         ${PROJECT_SOURCE_DIR}
         FILES
         ${PROJECT_SOURCE_DIR}/include/linux_stuff.hpp
         FILES
         ${PROJECT_SOURCE_DIR}/include/profiler.hpp
         FILES
         ${PROJECT_SOURCE_DIR}/include/loop_tester.hpp)

set_target_properties(perf_stuff PROPERTIES VERIFY_INTERFACE_HEADER_SETS ON)
