#include <benchmark/benchmark.h>
#include <boost/random/taus88.hpp>
#include <boost/random/mersenne_twister.hpp>

#include <random>
#include <vector>

#include "mcml_mcg59.h"
#include "splitmix64.hpp"

#define LATENCY

#define MCG59_C     302875106592253
#define MCG59_M     576460752303423488
#define MCG59_DEC_M 576460752303423487

constexpr int seed = 20260224;
constexpr std::uint64_t N = 1e6;

double NextMCG59(MCG59* randomGenerator)
{
	randomGenerator->value = (randomGenerator->value * randomGenerator->offset) & MCG59_DEC_M;
	return (double)(randomGenerator->value) / MCG59_M;
}

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
  std::uint64_t s0 = sm.next();
  std::uint64_t s1 = sm.next();
  std::vector<uint64_t> buffer(N);

  for (auto _ : state) {
    for (int i = 0; i < N; ++i) {
      std::uint64_t _s1 = s0;
      const std::uint64_t _s0 = s1;
      s0 = _s0;
      _s1 ^= _s1 << 23; // a
      s1 = _s1 ^ _s0 ^ (_s1 >> 17) ^ (_s0 >> 26); // b, c
      buffer[i] = s1 + _s0;
    }
    benchmark::DoNotOptimize(buffer.data());
    benchmark::ClobberMemory();
  }

  state.SetItemsProcessed(std::uint64_t(state.iterations()) * N);
}

static void mcg59_bm(benchmark::State& state) {

  MCG59 mcg59;
  InitMCG59(&mcg59, seed, 0, 1);

  for (auto _ : state) {
    auto val = NextMCG59(&mcg59);
    benchmark::DoNotOptimize(val);
  }
}

static void mcg59_throughput(benchmark::State& state) {

  MCG59 mcg59;
  InitMCG59(&mcg59, seed, 0, 1);
  std::vector<double> buffer(N);

  for (auto _ : state) {
    for (int i = 0; i < N; ++i) {
      buffer[i] = NextMCG59(&mcg59);
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
BENCHMARK(mcg59_bm);
#endif

#ifdef PERF
BENCHMARK(mt11213b_throughput);
BENCHMARK(taus88_throughput);
BENCHMARK(mt19937_throughput);
BENCHMARK(xorshift128plus_throughput);
BENCHMARK(mcg59_throughput);
#endif

BENCHMARK_MAIN();