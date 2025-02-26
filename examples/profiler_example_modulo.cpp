#include <unistd.h>

#include <cstdint>

#include "include/profiler.hpp"

using PerfStuff::Profile::ProfileBlock;
using PerfStuff::Profile::Profiler;

int64_t modulo(Profiler& profiler, int64_t num) {
  ProfileBlock a(num, profiler, 0);
  num = ++num % 7;
  return num;
}

int main() {
  int64_t num = 0;
  Profiler profiler;
  num = modulo(profiler, num + 1);
  num = modulo(profiler, num + 2);
  num = modulo(profiler, num + 3);
  num = modulo(profiler, num + 4);
  num = modulo(profiler, num + 5);
  num = modulo(profiler, num + 6);
  num = modulo(profiler, num + 7);
  return num;
}
