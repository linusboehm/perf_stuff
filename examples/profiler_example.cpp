#include <unistd.h>
#include <iostream>

#include "include/profiler.hpp"

using PerfStuff::Profile::Profiler;
using PerfStuff::Profile::ProfileBlock;

void foo(Profiler& profiler) {
  ProfileBlock a(1, profiler, 0);
  usleep(1000);
}

void print_stuff(Profiler& profiler) {
  ProfileBlock a(0, profiler, 20000);
  std::cout << "stuff" << std::endl;
  foo(profiler);
  std::cout << "stuff" << std::endl;
  foo(profiler);
}

int main() {
  Profiler profiler;
  print_stuff(profiler);
}

// stuff
// stuff
//
// Total time: 2.1858 ms (cpu freq 2476 MHz)
// Total mem: 0 MiB
// Total virt mem: 0 MiB
// Total page faults: 138 (bytes per fault: 0 bytes/fault)
//   ProfileBlock: 0 (#calls: 1)
//     Time: 0.019ms | 0.88% (2.1000ms 96.08% including callees)
//     Data: 0.019MB | 0.01GB/s
//     page faults: 0 (0 including callees)
//   ProfileBlock: 2 (#calls: 2)
//     Time: 2.081ms | 95.19%
//     page faults: 0
