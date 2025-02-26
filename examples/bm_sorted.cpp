#include <cstdint>
#include <cstdlib>
#include <numeric>
#include <random>
#include <unordered_map>
#include <vector>

#include "benchmark/benchmark.h"

static constexpr int64_t NUM_ELEMENTS = 1'000'000;
static constexpr int64_t NUM_UPDATES = 1000;
std::mt19937 mt_rand;

class SortedInsertFixture : public benchmark::Fixture {
 public:
  void SetUp(const ::benchmark::State&) final {
    std::vector<int64_t> unique_ids(NUM_ELEMENTS + NUM_UPDATES);
    std::iota(unique_ids.begin(), unique_ids.end(), 0);
    std::shuffle(unique_ids.begin(), unique_ids.end(), mt_rand);

    std::vector<int64_t> values(unique_ids.begin(), unique_ids.begin() + NUM_ELEMENTS);
    std::copy(values.begin(), values.end(), std::back_inserter(sorted_values_));
    std::sort(sorted_values_.begin(), sorted_values_.end(),
              [](const int64_t a, const int64_t b) { return a < b; });

    // construct update mapping
    std::transform(
        unique_ids.begin(), unique_ids.begin() + NUM_UPDATES, unique_ids.begin() + NUM_ELEMENTS,
        std::inserter(updates_, updates_.end()),
        [](const int64_t key, const int64_t value) { return std::make_pair(key, value); });
  }

  void TearDown(const ::benchmark::State&) final {}

  std::vector<int64_t> sorted_values_;
  std::unordered_map<int64_t, int64_t> updates_;
};

BENCHMARK_DEFINE_F(SortedInsertFixture, SetInsertion)(benchmark::State& state) {
  std::set<int64_t> s(sorted_values_.begin(), sorted_values_.end());
  for (auto _ : state) {
    for (const auto [curr_val, new_val] : updates_) {
      s.insert(new_val);
    }
    benchmark::ClobberMemory();
  }
  state.counters["elements"] = s.size();
  benchmark::DoNotOptimize(sorted_values_);
}

BENCHMARK_DEFINE_F(SortedInsertFixture, TopNSet)(benchmark::State& state) {
  std::uniform_int_distribution<int> distribution(0, NUM_ELEMENTS);
  const auto N = distribution(mt_rand);
  std::set<int64_t> s(sorted_values_.begin(), sorted_values_.end());
  std::vector<int64_t> top_n;
  for (auto _ : state) {
    std::copy_n(s.begin(), N, std::back_inserter(top_n));
    benchmark::ClobberMemory();
  }
  benchmark::DoNotOptimize(sorted_values_);
}

BENCHMARK_DEFINE_F(SortedInsertFixture, TopNVec)(benchmark::State& state) {
  std::uniform_int_distribution<int> distribution(0, NUM_ELEMENTS);
  const auto N = distribution(mt_rand);
  std::vector<int64_t> top_n;
  for (auto _ : state) {
    std::copy_n(sorted_values_.begin(), N, std::back_inserter(top_n));
    benchmark::ClobberMemory();
  }
  benchmark::DoNotOptimize(sorted_values_);
}

BENCHMARK_DEFINE_F(SortedInsertFixture, VectorSort)(benchmark::State& state) {
  for (auto _ : state) {
    for (const auto [curr_val, new_val] : updates_) {
      sorted_values_.push_back(new_val);
      std::sort(sorted_values_.begin(), sorted_values_.end());
    }
    benchmark::ClobberMemory();
  }
  benchmark::DoNotOptimize(sorted_values_);
  state.counters["elements"] = sorted_values_.size();
}

BENCHMARK_DEFINE_F(SortedInsertFixture, VectorInsert)(benchmark::State& state) {
  for (auto _ : state) {
    for (const auto [curr_val, new_val] : updates_) {
      auto it = std::lower_bound(sorted_values_.begin(), sorted_values_.end(), new_val);
      sorted_values_.insert(it, new_val);
    }
    benchmark::ClobberMemory();
  }
  benchmark::DoNotOptimize(sorted_values_);
  state.counters["elements"] = sorted_values_.size();
}

BENCHMARK_DEFINE_F(SortedInsertFixture, VectorRemove)(benchmark::State& state) {
  for (auto _ : state) {
    for (const auto [curr_val, new_val] : updates_) {
      auto it = std::lower_bound(sorted_values_.begin(), sorted_values_.end(), curr_val);
      if (it != sorted_values_.end() && *it == curr_val) {
        sorted_values_.erase(it);
      }
    }
    benchmark::ClobberMemory();
  }
  state.counters["elements"] = sorted_values_.size();
  benchmark::DoNotOptimize(sorted_values_);
}

BENCHMARK_DEFINE_F(SortedInsertFixture, VectorUpdate)(benchmark::State& state) {
  for (auto _ : state) {
    for (const auto& [curr_val, new_val] : updates_) {
      auto it = std::lower_bound(sorted_values_.begin(), sorted_values_.end(), curr_val);
      if (it != sorted_values_.end() && *it == curr_val) {
        sorted_values_.erase(it);
      }

      it = std::lower_bound(sorted_values_.begin(), sorted_values_.end(), new_val);
      sorted_values_.insert(it, new_val);
    }
    benchmark::ClobberMemory();
  }
  benchmark::DoNotOptimize(sorted_values_);
  state.counters["elements"] = sorted_values_.size();
}

BENCHMARK_DEFINE_F(SortedInsertFixture, VectorRotate)(benchmark::State& state) {
  for (auto _ : state) {
    for (const auto [existing, new_val] : updates_) {
      auto curr_loc = std::lower_bound(sorted_values_.begin(), sorted_values_.end(), existing);
      auto new_loc = std::lower_bound(sorted_values_.begin(), sorted_values_.end(), new_val);

      *curr_loc = new_val;

      if ((curr_loc == new_loc) || ((curr_loc + 1) == new_loc)) {
        continue;  // same location
      }

      auto start = std::min(curr_loc, new_loc);
      auto end = std::max(curr_loc + 1, new_loc);
      auto mid = (new_loc < curr_loc) ? curr_loc : curr_loc + 1;
      std::rotate(start, mid, end);
    }

    benchmark::ClobberMemory();
  }
  benchmark::DoNotOptimize(sorted_values_);
  state.counters["elements"] = sorted_values_.size();
}

// BENCHMARK_REGISTER_F(SortedInsertFixture, TopNSet)->Iterations(1);
// BENCHMARK_REGISTER_F(SortedInsertFixture, TopNVec)->Iterations(1);
// BENCHMARK_REGISTER_F(SortedInsertFixture, SetInsertion)->Iterations(1);
// BENCHMARK_REGISTER_F(SortedInsertFixture, VectorSort)->Iterations(1);
BENCHMARK_REGISTER_F(SortedInsertFixture, VectorInsert)->Iterations(1);
BENCHMARK_REGISTER_F(SortedInsertFixture, VectorRemove)->Iterations(1);
BENCHMARK_REGISTER_F(SortedInsertFixture, VectorUpdate)->Iterations(1);
BENCHMARK_REGISTER_F(SortedInsertFixture, VectorRotate)->Iterations(1);

BENCHMARK_MAIN();
