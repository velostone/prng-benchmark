#include <benchmark/benchmark.h>
#include <boost/random/taus88.hpp>
#include <boost/random/mersenne_twister.hpp>

#include <random>
#include <vector>

#include "splitmix64.hpp"

constexpr int seed = 20260224;
constexpr std::uint64_t N = 1e6;

static void mt11213b_bm(benchmark::State& state) {

  boost::random::mt11213b mt11213b(seed);

  for (auto _ : state) {
    auto val = mt11213b();
    benchmark::DoNotOptimize(val);
  }
}

static void mt11213b_throughput(benchmark::State& state) {

  boost::random::mt11213b mt11213b(seed);
  std::vector<uint64_t> buffer(N);

  for (auto _ : state) {
    for (int i = 0; i < N; ++i) {
      buffer[i] = mt11213b();
    }
    benchmark::DoNotOptimize(buffer.data());
    benchmark::ClobberMemory();
  }

  state.SetItemsProcessed(std::uint64_t(state.iterations()) * N);
}

static void taus88_bm(benchmark::State& state) {

  boost::random::taus88 taus88(seed);

  for (auto _ : state) {
    auto val = taus88();
    benchmark::DoNotOptimize(val);
  }
}

static void taus88_throughput(benchmark::State& state) {

  boost::random::mt11213b taus88(seed);
  std::vector<uint64_t> buffer(N);

  for (auto _ : state) {
    for (int i = 0; i < N; ++i) {
      buffer[i] = taus88();
    }
    benchmark::DoNotOptimize(buffer.data());
    benchmark::ClobberMemory();
  }

  state.SetItemsProcessed(std::uint64_t(state.iterations()) * N);
}

static void mt19937_bm(benchmark::State& state) {

  std::mt19937 mt19937(seed);

  for (auto _ : state) {
    auto val = mt19937();
    benchmark::DoNotOptimize(val);
  }
}

static void mt19937_throughput(benchmark::State& state) {

  boost::random::mt11213b mt19937(seed);
  std::vector<uint64_t> buffer(N);

  for (auto _ : state) {
    for (int i = 0; i < N; ++i) {
      buffer[i] = mt19937();
    }
    benchmark::DoNotOptimize(buffer.data());
    benchmark::ClobberMemory();
  }

  state.SetItemsProcessed(std::uint64_t(state.iterations()) * N);
}

static void xorshift128plus_bm(benchmark::State& state) {

  SplitMix64 sm(seed);
  std::uint64_t s[2];
  s[0] = sm.next();
  s[1] = sm.next();

  for (auto _ : state) {
		std::uint64_t s1 = s[0];
		const std::uint64_t s0 = s[1];
		s[0] = s0;
		s1 ^= s1 << 23; // a
		s[1] = s1 ^ s0 ^ (s1 >> 17) ^ (s0 >> 26); // b, c
		auto val = s[1] + s0;
    benchmark::DoNotOptimize(val);
  }
}

static void xorshift128plus_throughput(benchmark::State& state) {

  SplitMix64 sm(seed);
  std::uint64_t s[2];
  s[0] = sm.next();
  s[1] = sm.next();
  std::vector<uint64_t> buffer(N);

  for (auto _ : state) {
    for (int i = 0; i < N; ++i) {
      std::uint64_t s1 = s[0];
      const std::uint64_t s0 = s[1];
      s[0] = s0;
      s1 ^= s1 << 23; // a
      s[1] = s1 ^ s0 ^ (s1 >> 17) ^ (s0 >> 26); // b, c
      buffer[i] = s[1] + s0;
    }
    benchmark::DoNotOptimize(buffer.data());
    benchmark::ClobberMemory();
  }

  state.SetItemsProcessed(std::uint64_t(state.iterations()) * N);
}

#ifdef LATENCY
BENCHMARK(mt11213b_bm);
BENCHMARK(taus88_bm);
BENCHMARK(mt19937_bm);
BENCHMARK(xorshift128plus_bm);
#endif

BENCHMARK(mt11213b_throughput);
BENCHMARK(taus88_throughput);
BENCHMARK(mt19937_throughput);
BENCHMARK(xorshift128plus_throughput);

BENCHMARK_MAIN();

#ifdef INFO
int main() {

  /* ---------------- mt11213b ---------------- */

  boost::random::mt11213b mt11213b(seed);

  std::cout << boost::random::mt11213b::state_size << "\n"; // uint32 elems in state
  std::cout << sizeof(boost::random::mt11213b::result_type) << "\n"; // result size in bytes

  /* ---------------- taus88 ---------------- */

  boost::random::taus88 taus88(seed);

  std::cout << sizeof(boost::random::taus88) << "\n"; // state size in bytes
  std::cout << sizeof(boost::random::taus88::result_type) << "\n"; // result size in bytes

  /* ---------------- mt19937 ---------------- */

  std::mt19937 mt19937(seed);

  std::cout << std::mt19937::state_size << "\n"; // uint32 elems in state
  std::cout << sizeof(std::mt19937::result_type) << "\n"; // result size in bytes

  /* ---------------- xorshift128+ ---------------- */

  XorShift128Plus xorshift128plus(seed);

  std::cout << sizeof(XorShift128Plus) << "\n"; // state size in bytes
  std::cout << sizeof(XorShift128Plus::result_type) << "\n"; // state size in bytes
}
#endif