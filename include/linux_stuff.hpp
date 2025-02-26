#pragma once

#include <fcntl.h>  // open
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <x86intrin.h>

#include <cstdint>

#include "include/utils.hpp"

char* do_mmap(const size_t size);

namespace PerfStuff::OS {

double tsc_to_sec(double tsc_elapsed, uint64_t cpu_freq);
double tsc_to_msec(double tsc_elapsed, uint64_t cpu_freq);

struct buffer {
  size_t Count;
  uint8_t* Data;
};

static constexpr uint64_t OS_TIMER_FREQU = 1000000;

uint64_t get_os_timer();
uint64_t read_page_fault_cnt();
uint64_t virt_mem_usage();
uint64_t mem_usage();
int32_t read_random_bytes(uint64_t Count, buffer buffer, uint64_t offset);

INLINE uint64_t get_tsc(void) {
  return __rdtsc();
}

uint64_t get_approx_cpu_freq();

void* os_allocate(size_t ByteCount);

struct Platform {
  // TODO(lboehm): fix this
  uint64_t large_page_size_;
  uint64_t cpu_freq_ = get_approx_cpu_freq();
  inline uint64_t get_cpu_freq_() {
    return cpu_freq_;
  }
  inline uint64_t get_large_page_size() {
    return large_page_size_;
  }
};

inline void fill_random_bytes(buffer Dest);

}  // namespace PerfStuff::OS
