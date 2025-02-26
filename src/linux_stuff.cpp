#include "include/linux_stuff.hpp"

#include <unistd.h>

#include <cstdint>
#include <fstream>
#include <iostream>

char* do_mmap(const size_t size) {
  return static_cast<char*>(
      mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
}

namespace PerfStuff::OS {

double tsc_to_sec(const double tsc_elapsed, const uint64_t cpu_freq) {
  return tsc_elapsed / cpu_freq;
}
double tsc_to_msec(const double tsc_elapsed, const uint64_t cpu_freq) {
  return tsc_to_sec(tsc_elapsed, cpu_freq) * 1000;
}

uint64_t get_os_timer() {
  struct timeval Value;
  gettimeofday(&Value, 0);
  return OS_TIMER_FREQU * (uint64_t)Value.tv_sec + (uint64_t)Value.tv_usec;
}

uint64_t read_page_fault_cnt() {
  struct rusage Usage = {};
  getrusage(RUSAGE_SELF, &Usage);
  return Usage.ru_minflt + Usage.ru_majflt;
}

uint64_t virt_mem_usage() {
  std::ifstream statm("/proc/self/statm");
  size_t size_pages;
  statm >> size_pages;
  int64_t page_size_kb = sysconf(_SC_PAGESIZE);
  return size_pages * page_size_kb;
}

uint64_t mem_usage() {
  std::ifstream status("/proc/self/status");
  std::string line;

  while (std::getline(status, line)) {
    if (line.rfind("VmRSS:", 0) == 0) {
      // Example line: "VmRSS:	  1048576 kB"
      std::istringstream iss(line);
      std::string label, unit;
      int64_t rss_kb;
      iss >> label >> rss_kb >> unit;
      return rss_kb * 1024;
    }
  }
  return 0;
}

// int32_t read_random_bytes(uint64_t Count, buffer buffer, uint64_t offset) {
//   int DevRandom = open("/dev/urandom", O_RDONLY);
//   int32_t Result = (read(DevRandom, buffer.Data + offset, buffer.Count) == Count);
//   close(DevRandom);
//
//   return Result;
// }

uint64_t get_approx_cpu_freq() {
  static constexpr uint64_t wait_us = 100'000;
  static constexpr uint64_t os_freq = OS_TIMER_FREQU;

  const uint64_t tsc_begin = get_tsc();
  const uint64_t os_begin = get_os_timer();
  usleep(wait_us);

  const uint64_t os_elapsed = get_os_timer() - os_begin;
  const uint64_t tsc_end = get_tsc();
  const uint64_t tsc_elapsed = tsc_end - tsc_begin;
  ASSERT(os_elapsed) << "no time elapsed to measure cpu freq";
  const uint64_t cpu_freq = os_freq * tsc_elapsed / os_elapsed;
  ASSERT(cpu_freq) << "cpu freq estimation failed";
  return cpu_freq;
}

#define TimeFunction TimeBlock(__func__)

// void* os_allocate(size_t ByteCount) {
//   void* Result = mmap(0, ByteCount, PROT_READ | PROT_WRITE, MAP_ANONYMOUS, 0, 0);
//   return Result;
// }
//
// inline void fill_random_bytes(buffer Dest) {
//   uint64_t MaxRandCount = std::numeric_limits<ssize_t>::max();
//   uint64_t AtOffset = 0;
//   while (AtOffset < Dest.Count) {
//     uint64_t ReadCount = Dest.Count - AtOffset;
//     if (ReadCount > MaxRandCount) {
//       ReadCount = MaxRandCount;
//     }
//
//     ReadOSRandomBytes(ReadCount, Dest, AtOffset);
//     AtOffset += ReadCount;
//   }
// }

}  // namespace PerfStuff::OS
