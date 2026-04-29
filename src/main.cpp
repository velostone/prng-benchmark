#include <benchmark/benchmark.h>
#include <boost/random/taus88.hpp>
#include <boost/random/mersenne_twister.hpp>

#include <cstdint>
#include <random>
#include <vector>

#include "mcml_mcg59.h"
#include "xoshiro256plus.hpp"
#include "xoshiro256plusplus.hpp"
#include "xoshiro128plus.hpp"

#define LATENCY 

constexpr int seed = 20260224;
constexpr std::uint64_t N = 1e6;

static void mt11213b_latency(benchmark::State& state) {

  boost::random::mt11213b mt11213b(seed);

  for (auto _ : state) {
    auto val = mt11213b();
    benchmark::DoNotOptimize(val);
  }
}

static void mt11213b_throughput(benchmark::State& state) {

  boost::random::mt11213b mt11213b(seed);
  std::vector<std::uint64_t> buffer(N);

  for (auto _ : state) {
    for (int i = 0; i < N; ++i) {
      buffer[i] = mt11213b();
    }
    benchmark::DoNotOptimize(buffer.data());
    benchmark::ClobberMemory();
  }

  state.SetItemsProcessed(std::uint64_t(state.iterations()) * N);
}

static void taus88_latency(benchmark::State& state) {

  boost::random::taus88 taus88(seed);

  for (auto _ : state) {
    auto val = taus88();
    benchmark::DoNotOptimize(val);
  }
}

static void taus88_throughput(benchmark::State& state) {

  boost::random::taus88 taus88(seed);
  std::vector<std::uint64_t> buffer(N);

  for (auto _ : state) {
    for (int i = 0; i < N; ++i) {
      buffer[i] = taus88();
    }
    benchmark::DoNotOptimize(buffer.data());
    benchmark::ClobberMemory();
  }

  state.SetItemsProcessed(std::uint64_t(state.iterations()) * N);
}

static void mt19937_latency(benchmark::State& state) {

  std::mt19937 mt19937(seed);

  for (auto _ : state) {
    auto val = mt19937();
    benchmark::DoNotOptimize(val);
  }
}

static void mt19937_throughput(benchmark::State& state) {

  std::mt19937 mt19937(seed);
  std::vector<std::uint64_t> buffer(N);

  for (auto _ : state) {
    for (int i = 0; i < N; ++i) {
      buffer[i] = mt19937();
    }
    benchmark::DoNotOptimize(buffer.data());
    benchmark::ClobberMemory();
  }

  state.SetItemsProcessed(std::uint64_t(state.iterations()) * N);
}

static void mcg59_latency(benchmark::State& state) {

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
  std::vector<std::uint64_t> buffer(N);

  for (auto _ : state) {
    for (int i = 0; i < N; ++i) {
      buffer[i] = NextMCG59(&mcg59);
    }
    benchmark::DoNotOptimize(buffer.data());
    benchmark::ClobberMemory();
  }

  state.SetItemsProcessed(std::uint64_t(state.iterations()) * N);
}

static void xoshiro256plus_latency(benchmark::State& state) {

  XoShiRo256Plus xoshiro(seed);

  for (auto _ : state) {
      auto val = xoshiro();
      benchmark::DoNotOptimize(val);
    }
}

static void xoshiro256plus_throughput(benchmark::State& state) {

  XoShiRo256Plus xoshiro(seed);
  std::vector<std::uint64_t> buffer(N);

  for (auto _ : state) {
    for (int i = 0; i < N; ++i) {
      buffer[i] = xoshiro();
    }
    benchmark::DoNotOptimize(buffer.data());
    benchmark::ClobberMemory();
  }

  state.SetItemsProcessed(std::uint64_t(state.iterations()) * N);
}

static void xoshiro256pp_latency(benchmark::State& state) {

  XoShiRo256PlusPlus xoshiro(seed);

  for (auto _ : state) {
      auto val = xoshiro();
      benchmark::DoNotOptimize(val);
    }
}

static void xoshiro256pp_throughput(benchmark::State& state) {

  XoShiRo256PlusPlus xoshiro(seed);
  std::vector<std::uint64_t> buffer(N);

  for (auto _ : state) {
    for (int i = 0; i < N; ++i) {
      buffer[i] = xoshiro();
    }
    benchmark::DoNotOptimize(buffer.data());
    benchmark::ClobberMemory();
  }

  state.SetItemsProcessed(std::uint64_t(state.iterations()) * N);
}

static void xoshiro128plus_latency(benchmark::State& state) {

  XoShiRo128Plus xoshiro(seed);

  for (auto _ : state) {
      auto val = xoshiro();
      benchmark::DoNotOptimize(val);
    }
}

static void xoshiro128plus_throughput(benchmark::State& state) {

  XoShiRo128Plus xoshiro(seed);
  std::vector<std::uint32_t> buffer(N);

  for (auto _ : state) {
    for (int i = 0; i < N; ++i) {
      buffer[i] = xoshiro();
    }
    benchmark::DoNotOptimize(buffer.data());
    benchmark::ClobberMemory();
  }

  state.SetItemsProcessed(std::uint64_t(state.iterations()) * N);
}

#ifdef LATENCY
BENCHMARK(mt11213b_latency);
BENCHMARK(taus88_latency);
BENCHMARK(mt19937_latency);
BENCHMARK(mcg59_latency);
BENCHMARK(xoshiro256plus_latency);
BENCHMARK(xoshiro256pp_latency);
BENCHMARK(xoshiro128plus_latency);
#endif

#ifdef THROUGHPUT
BENCHMARK(mt11213b_throughput);
BENCHMARK(taus88_throughput);
BENCHMARK(mt19937_throughput);
BENCHMARK(mcg59_throughput);
BENCHMARK(xoshiro256plus_throughput);
BENCHMARK(xoshiro256pp_throughput);
BENCHMARK(xoshiro128plus_throughput);
#endif

BENCHMARK_MAIN();
