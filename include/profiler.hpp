#pragma once

#include <sys/time.h>
#include <x86intrin.h>

#include <array>
#include <cstdint>

#include "include/linux_stuff.hpp"

namespace PerfStuff::Profile {

struct ScopeStats {
  uint64_t tsc_excl_callees_{};
  uint64_t tsc_incl_callees_{};
  uint64_t pf_excl_callees_{};
  uint64_t pf_incl_callees_{};
  uint64_t cnt_{};
  uint64_t byte_cnt_{};
};

struct Profiler {
  Profiler();
  ~Profiler();

  void print_scopes();
  void print_scope(const size_t id, const ScopeStats& scope);

  OS::Platform platform_;
  std::array<ScopeStats, 32> scopes_;
  int32_t current_parent_ = -1;
  uint64_t tsc_start_{};
  uint64_t tsc_total_{};
  uint64_t pf_start_{};
  uint64_t pf_total_{};
  uint64_t mem_start_{};
  uint64_t vmem_start_{};
};

struct ProfileScope {
  ProfileScope(uint32_t id, Profiler& profiler, uint64_t ByteCount = 0);
  ~ProfileScope();

  uint32_t id_;
  uint64_t prev_elapsed_incl_callees_;
  uint64_t prev_pf_elapsed_incl_callees_;
  uint64_t tsc_start_;
  uint32_t parent_id_;
  uint32_t pf_start_;
  Profiler& profiler_;
};

}  // namespace PerfStuff::Profile
