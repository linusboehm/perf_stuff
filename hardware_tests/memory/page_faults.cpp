#include <cstring>

#include "include/profiler.hpp"

static constexpr auto ONE_GiB = (1U << 30U);

int sum(char* array, std::size_t size) {
  int cnt = 0;
  for (std::size_t i = 0; i < size; ++i) { cnt += static_cast<int8_t>(array[i]); }
  return cnt;
}

int main() {
  static constexpr int total_gb = 5;

  char* addresses[total_gb];
  size_t total = 0;

  PerfStuff::Profile::Profiler profiler;
  {
    PerfStuff::Profile::ProfileBlock a(0, profiler);
    for (int i = 0; i < total_gb; ++i) { addresses[i] = do_mmap(ONE_GiB); }
  }
  for (const int run : {1, 2}) {
    PerfStuff::Profile::ProfileBlock a(run, profiler);
    for (int i = 0; i < total_gb; ++i) {
      // std::memset(addresses[i], 0, ONE_GiB);
      total += sum(addresses[i], ONE_GiB);
    }
  }
  for (const int run : {4, 5}) {
    PerfStuff::Profile::ProfileBlock a(run, profiler);
    for (int i = 0; i < total_gb; ++i) {
      std::memset(addresses[i], 0, ONE_GiB);
      total += sum(addresses[i], ONE_GiB);
    }
  }
  return total;
}
