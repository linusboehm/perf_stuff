#include "include/loop_tester.hpp"

#include <cstdint>
#include <iostream>
#include <limits>

#include "include/linux_stuff.hpp"
#include "include/utils.hpp"

namespace PerfStuff::LoopTester {

void SampleStats::print(const uint64_t cpu_freq) const {
  min_.print_value("Min", cpu_freq);
  std::cout << std::endl;
  max_.print_value("Max", cpu_freq);
  std::cout << std::endl;
  total_.print_value("Avg", cpu_freq);
  std::cout << std::endl;
}

void LoopTester::new_test_loop(uint64_t expected_byte_count, uint32_t try_for_secs) {
  if (mode_ == TestMode::UNINITIALIZED) {
    mode_ = TestMode::FIRST_RUN;
    expected_byte_count_ = expected_byte_count;
    cpu_freq_ = platform_.get_cpu_freq_();
    print_new_mins_ = true;
    sample_stats_.min_[StatType::TSC_ELAPSED] = std::numeric_limits<uint64_t>::max();
  } else if (mode_ == TestMode::DONE) {
    mode_ = TestMode::FIRST_RUN;
    ASSERT(expected_byte_count_ == expected_byte_count) << "expected_byte_count changed";
  }

  try_for_cycles_ = try_for_secs * cpu_freq_;
  tsc_start_ = OS::get_tsc();
}

void LoopTester::init_samples() {
  samples_[StatType::PAGE_FAULTS] -= OS::read_page_fault_cnt();
  samples_[StatType::TSC_ELAPSED] -= OS::get_tsc();
}

void LoopTester::stop_samples(uint64_t ByteCount) {
  samples_[StatType::TSC_ELAPSED] += OS::get_tsc();
  samples_[StatType::PAGE_FAULTS] += OS::read_page_fault_cnt();
  samples_[StatType::BYTE_CNT] += ByteCount;
}

int32_t LoopTester::is_testing() {
  uint64_t tsc_now = OS::get_tsc();
  if (mode_ == TestMode::FIRST_RUN) {
    mode_ = TestMode::TESTING;
  } else if (mode_ == TestMode::TESTING) {
    ASSERT(samples_[StatType::BYTE_CNT] == expected_byte_count_) << "Processed byte count mismatch";
    sample_stats_.total_.sample(samples_);
    sample_stats_.max_[StatType::TSC_ELAPSED] =
        std::max(sample_stats_.max_[StatType::TSC_ELAPSED], samples_[StatType::TSC_ELAPSED]);

    if (sample_stats_.min_[StatType::TSC_ELAPSED] > samples_[StatType::TSC_ELAPSED]) {
      sample_stats_.min_ = samples_;

      tsc_start_ = tsc_now;  // reset clock

      if (print_new_mins_) {
        printf("                                                         \r");
        sample_stats_.min_.print_value("new min", cpu_freq_);
      }
    }

    samples_ = {};
  }

  if ((tsc_now - tsc_start_) > try_for_cycles_) {
    mode_ = TestMode::DONE;
    printf("                                                                          \r");
    sample_stats_.print(cpu_freq_);
  }
  return mode_ == TestMode::TESTING;
}

void AbstractTestFunc::measure(RunParams& params) {
  tester_.new_test_loop(params.bytes_, 10);
  setup(params);
  while (tester_.is_testing()) {
    tester_.init_samples();
    run(params);
    tester_.stop_samples(params.bytes_);
  }
  cleanup(params);
}
std::string AbstractTestFunc::get_name() const {
  return name_;
}

}  // namespace PerfStuff::LoopTester
