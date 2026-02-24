#include <benchmark/benchmark.h>
#include <boost/random/taus88.hpp>
#include <boost/random/mersenne_twister.hpp>

#include <random>

#include "splitmix64.hpp"

constexpr int seed = 20260224;

static void mt11213b_bm(benchmark::State& state) {

  boost::random::mt11213b mt11213b(seed);

  for (auto _ : state) {
    auto val = mt11213b();
    benchmark::DoNotOptimize(val);
  }
}

static void taus88_bm(benchmark::State& state) {

  boost::random::taus88 taus88(seed);

  for (auto _ : state) {
    auto val = taus88();
    benchmark::DoNotOptimize(val);
  }
}

static void mt19937_bm(benchmark::State& state) {

  std::mt19937 mt19937(seed);

  for (auto _ : state) {
    auto val = mt19937();
    benchmark::DoNotOptimize(val);
  }
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

BENCHMARK(mt11213b_bm);
BENCHMARK(taus88_bm);
BENCHMARK(mt19937_bm);
BENCHMARK(xorshift128plus_bm);

BENCHMARK_MAIN();


#ifdef INFO
int main() {

  /* ---------------- mt11213b ---------------- */

  boost::random::mt11213b mt11213b(seed);

  for (int i = 0; i < 50; ++i)
    std::cout << nextFloat<boost::random::mt11213b>(mt11213b, 8) << "\n";

  std::cout << boost::random::mt11213b::state_size << "\n"; // uint32 elems in state
  std::cout << sizeof(boost::random::mt11213b::result_type) << "\n"; // result size in bytes

  /* ---------------- taus88 ---------------- */

  boost::random::taus88 taus88(seed);

  for (int i = 0; i < 50; ++i)
    std::cout << nextFloat<boost::random::taus88>(taus88, 8) << "\n";

  std::cout << sizeof(boost::random::taus88) << "\n"; // state size in bytes
  std::cout << sizeof(boost::random::taus88::result_type) << "\n"; // result size in bytes

  /* ---------------- mt19937 ---------------- */

  std::mt19937 mt19937(seed);

  for (int i = 0; i < 50; ++i)
    std::cout << nextFloat<std::mt19937>(mt19937, 8) << "\n";

  std::cout << std::mt19937::state_size << "\n"; // uint32 elems in state
  std::cout << sizeof(std::mt19937::result_type) << "\n"; // result size in bytes

  /* ---------------- xorshift128+ ---------------- */

  XorShift128Plus xorshift128plus(seed);

  for (int i = 0; i < 50; ++i)
    std::cout << nextFloat<XorShift128Plus>(xorshift128plus, 40) << "\n";

  std::cout << sizeof(XorShift128Plus) << "\n"; // state size in bytes
  std::cout << sizeof(XorShift128Plus::result_type) << "\n"; // state size in bytes
}
#endif