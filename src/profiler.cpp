#include "include/profiler.hpp"

#include <cstdint>
#include <cstdio>

#include "include/linux_stuff.hpp"
#include "include/utils.hpp"

namespace PerfStuff::Profile {

Profiler::Profiler() {
  tsc_start_ = OS::get_tsc();
  mem_start_ = OS::mem_usage();
  vmem_start_ = OS::virt_mem_usage();
}

Profiler::~Profiler() {
  const auto tsc_end = OS::get_tsc();
  const auto pf_end = OS::read_page_fault_cnt();
  const auto mem_end = OS::mem_usage() - mem_start_;
  const auto vmem_end = OS::virt_mem_usage() - vmem_start_;
  const auto cpu_freq = platform_.get_cpu_freq_();
  tsc_total_ = tsc_end - tsc_start_;
  pf_total_ = pf_end - pf_start_;

  // printf("\nTotal time: %0.4f ms (cpu freq %lu MHz)\n", OS::tsc_to_msec(tsc_total_, cpu_freq),
  //        (cpu_freq / (1 << 20)));
  printf("\nTotal time: %0.4f ms\n", OS::tsc_to_msec(tsc_total_, cpu_freq));
  printf("Total phys mem: %lu MiB\n", mem_end / (1 << 20));
  printf("Total virt mem: %lu MiB\n", vmem_end / (1 << 20));
  printf("Total page faults: %lu (bytes per fault: %lu bytes/fault)\n", pf_total_,
         vmem_end / pf_total_);

  print_scopes();
}

void Profiler::print_scope(const size_t id, const ScopeStats& scope) {
  const auto cpu_freq = platform_.get_cpu_freq_();
  const auto pct = 100.0 * (static_cast<double>(scope.tsc_excl_callees_) / tsc_total_);
  auto elapsed_excl = OS::tsc_to_sec(scope.tsc_excl_callees_, cpu_freq);
  std::string unit = "s";
  for (const std::string u : {"ms", "us", "ns"}) {
    if (elapsed_excl > 1) {
      break;
    }
    elapsed_excl *= 1000;
    unit = u;
  }
  printf("  %lu. ProfileScope: ", id);
  printf(" #calls: %lu", scope.cnt_);
  printf("\n    Time: %.3f%s | cycles: %lu | %.2f%%", elapsed_excl, unit.c_str(),
         scope.tsc_incl_callees_, pct);
  if (scope.tsc_incl_callees_ != scope.tsc_excl_callees_) {
    auto elapsed_incl = OS::tsc_to_sec(scope.tsc_incl_callees_, cpu_freq);
    std::string unit = "s";
    for (const std::string u : {"ms", "us", "ns"}) {
      if (elapsed_incl > 1) {
        break;
      }
      elapsed_incl *= 1000;
      unit = u;
    }
    const auto incl_callees_pct =
        100.0 * (static_cast<double>(scope.tsc_incl_callees_) / static_cast<double>(tsc_total_));
    printf(" (%.4f%s %.2f%% including callees)", elapsed_incl, unit.c_str(), incl_callees_pct);
  }
  const auto secs = OS::tsc_to_sec(scope.tsc_incl_callees_, cpu_freq);
  const auto bytes_per_sec = static_cast<double>(scope.byte_cnt_) / secs;
  const auto mb_total = static_cast<double>(scope.byte_cnt_) / ONE_MEGA_BYTE;
  const auto gb_per_sec = bytes_per_sec / ONE_GIGA_BYTE;

  if (mb_total != 0) {
    printf("\n    Data: %.3fMB | %.2fGB/s", mb_total, gb_per_sec);
  }
  printf("\n    page faults: %lu", scope.pf_excl_callees_);
  if (scope.tsc_incl_callees_ != scope.tsc_excl_callees_) {
    printf(" (%lu including callees)", scope.pf_incl_callees_);
  }
  printf("\n");
}

void Profiler::print_scopes() {
  for (size_t i = 0; i < scopes_.size(); i++) {
    const auto& scope = scopes_[i];
    if (scope.cnt_) {
      print_scope(i, scope);
    }
  }
}

ProfileScope::ProfileScope(uint32_t id, Profiler& profiler, uint64_t bytes) : profiler_(profiler) {
  parent_id_ = profiler_.current_parent_;

  ASSERT(id < profiler_.scopes_.size())
      << "id " << id << " is larger than available scopes: " << profiler_.scopes_.size();
  id_ = id;

  ScopeStats& scope = profiler_.scopes_[id_];
  prev_elapsed_incl_callees_ = scope.tsc_incl_callees_;
  prev_pf_elapsed_incl_callees_ = scope.pf_incl_callees_;
  scope.byte_cnt_ += bytes;

  profiler_.current_parent_ = id_;
  pf_start_ = OS::read_page_fault_cnt();
  tsc_start_ = OS::get_tsc();
}

ProfileScope::~ProfileScope() {
  uint64_t elapsed = OS::get_tsc() - tsc_start_;
  uint64_t elapsed_pf = OS::read_page_fault_cnt() - pf_start_;
  profiler_.current_parent_ = parent_id_;

  if (parent_id_ != -1) {
    ScopeStats& parent = profiler_.scopes_[parent_id_];
    parent.tsc_excl_callees_ -= elapsed;
    parent.pf_excl_callees_ -= elapsed_pf;
  }
  ScopeStats& scope = profiler_.scopes_[id_];

  scope.tsc_excl_callees_ += elapsed;
  scope.pf_excl_callees_ += elapsed_pf;
  scope.tsc_incl_callees_ = prev_elapsed_incl_callees_ + elapsed;
  scope.pf_incl_callees_ = prev_pf_elapsed_incl_callees_ + elapsed_pf;
  ++scope.cnt_;
}

}  // namespace PerfStuff::Profile
