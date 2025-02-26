#pragma once

#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <limits>
#include <string>

#include "include/linux_stuff.hpp"

namespace PerfStuff::LoopTester {

enum class TestMode {
  UNINITIALIZED,
  FIRST_RUN,
  TESTING,
  DONE,
};

enum class StatType {
  TSC_ELAPSED,
  PAGE_FAULTS,
  BYTE_CNT,

  NR_TYPES,  // has to be last!!
};

constexpr auto to_underlying(StatType value) {
  return static_cast<std::underlying_type_t<StatType>>(value);
}

template<typename T>
class LoopSamples {
 public:
  LoopSamples() = default;
  LoopSamples(T sample_init_value) {
    samples_.fill(sample_init_value);
  }

  T& operator[](StatType value) {
    return samples_[to_underlying(value)];
  }
  const T& operator[](StatType value) const {
    return samples_[to_underlying(value)];
  }
  void print_value(const std::string& Label, const uint64_t cpu_freq) const;

  size_t size() const {
    return samples_.size();
  }
  void sample(const LoopSamples<uint64_t>& samples) {
    ++nr_iterations_;
    for (uint32_t i = 0; i < samples.size(); ++i) {
      const auto repval = StatType(i);
      samples_[i] += samples[repval];
    }
  }

 private:
  LoopSamples<double> get_averaged_stats(const uint64_t cpu_freq) const;

  uint64_t nr_iterations_ = 0;
  std::array<T, to_underlying(StatType::NR_TYPES)> samples_{};
};

template<typename T>
void LoopSamples<T>::print_value(const std::string& Label, const uint64_t cpu_freq) const {
  const auto per_run_stats = get_averaged_stats(cpu_freq);
  const auto elapsed = per_run_stats[StatType::TSC_ELAPSED];
  auto time = OS::tsc_to_sec(elapsed, cpu_freq);
  std::string unit = "s";
  const auto gbps = per_run_stats[StatType::BYTE_CNT] / (ONE_GIGA_BYTE * time);
  printf("%s: %.0f cycles ", Label.c_str(), per_run_stats[StatType::TSC_ELAPSED]);
  for (const std::string u : {"ms", "us", "ns"}) {
    if (time > 1) {
      break;
    }
    time *= 1000;
    unit = u;
  }
  printf("(%.4f %s); BW: %.4f GB/s     ", time, unit.c_str(), gbps);

  // double kb_per_fault = 0;
  // if (per_run_stats[StatType::PAGE_FAULTS] > 0) {
  //   kb_per_fault =
  //       per_run_stats[StatType::BYTE_CNT] / (per_run_stats[StatType::PAGE_FAULTS] * 1024.0);
  // }
  // printf("; page faults: %0.4f (%0.4fk/fault)", per_run_stats[StatType::PAGE_FAULTS], kb_per_fault);
}

template<typename T>
LoopSamples<double> LoopSamples<T>::get_averaged_stats(const uint64_t cpu_freq) const {
  LoopSamples<double> averaged_stats;
  const uint64_t nr_tests = std::max(nr_iterations_, 1UL);

  for (uint32_t i = 0; i < samples_.size(); ++i) {
    averaged_stats[StatType(i)] = static_cast<double>(samples_[i]) / nr_tests;
  }
  return averaged_stats;
}

struct SampleStats {
  void print(const uint64_t cpu_freq) const;
  LoopSamples<uint64_t> total_{};
  LoopSamples<uint64_t> min_{std::numeric_limits<uint64_t>::max()};
  LoopSamples<uint64_t> max_{};
};

class LoopTester {
 public:
  void new_test_loop(uint64_t expected_byte_count, uint32_t SecondsToTry = 10);
  void init_samples();
  void stop_samples(uint64_t ByteCount);
  int32_t is_testing();

 private:
  uint64_t expected_byte_count_ = 0;
  uint64_t cpu_freq_ = 0;
  uint64_t try_for_cycles_ = 0;
  uint64_t tsc_start_ = 0;

  TestMode mode_ = TestMode::UNINITIALIZED;
  bool print_new_mins_ = false;

  LoopSamples<uint64_t> samples_{};
  SampleStats sample_stats_{};
  OS::Platform platform_;
};

struct RunParams {
  size_t bytes_;
  // buffer Dest;
};

class AbstractTestFunc {
 public:
  AbstractTestFunc(const std::string& name) : name_(name) {}
  void measure(RunParams& params);
  std::string get_name() const;
  virtual ~AbstractTestFunc() = default;

 protected:
  virtual void run(RunParams& params) = 0;
  virtual void setup(RunParams& params) {}
  virtual void cleanup(RunParams& params) {}

 private:
  std::string name_;
  LoopTester tester_;
};

}  // namespace PerfStuff::LoopTester
