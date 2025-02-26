#include <cstdint>
#include <cstring>

#include "include/profiler.hpp"

static constexpr auto ONE_GiB = (1U << 30U);
static constexpr auto THREE_GiB = 3 * ONE_GiB;

uint64_t read(char* array, std::size_t size) {
  auto data64 = reinterpret_cast<const uint64_t*>(array);
  uint64_t res = 0;
  for (std::size_t i = 0; i < size / sizeof(uint64_t);) {
    res += data64[i++];
    res += data64[i++];
    res += data64[i++];
    res += data64[i++];
  }
  return res;
}

int main() {
  char* address;
  uint64_t result = 0;

  PerfStuff::Profile::Profiler profiler;
  // ///////////////
  // ALLOCATE MEMORY
  {
    PerfStuff::Profile::ProfileScope a(0, profiler);
    address = do_mmap(THREE_GiB);
  }

  // ///////////////
  // READ MEMORY (touch every byte)
  for (const int run : {1, 2}) {
    PerfStuff::Profile::ProfileScope a(run, profiler);
    result += read(address, THREE_GiB);
  }

  // ///////////////
  // WRITE MEMORY
  for (const int run : {3, 4}) {
    PerfStuff::Profile::ProfileScope a(run, profiler);
    std::memset(address, 1, THREE_GiB);
  }
  result += read(address, THREE_GiB);
  std::cout << "Result: "<< result << '\n';
}
