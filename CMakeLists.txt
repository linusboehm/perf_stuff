# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

cmake_minimum_required(VERSION 3.25)

project(perf_stuff LANGUAGES CXX)

enable_language(ASM_NASM)
set(CMAKE_ASM_NASM_FLAGS "-f elf64")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
  googlebenchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG v1.9.0
  EXCLUDE_FROM_ALL)

set(BENCHMARK_ENABLE_TESTING OFF)
FetchContent_MakeAvailable(googlebenchmark)

add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(hardware_tests/execution)
add_subdirectory(hardware_tests/memory)
