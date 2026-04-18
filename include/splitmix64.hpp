#pragma once

#include <cstdint>

static inline std::uint64_t rotl(const std::uint64_t x, int k) {
	return (x << k) | (x >> (64 - k));
}

class SplitMix64 {
public:
	explicit SplitMix64(std::uint64_t seed) : state(seed) { };

	inline std::uint64_t next() {
		std::uint64_t z = (state += 0x9e3779b97f4a7c15);
		z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
		z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
		return z ^ (z >> 31);
	}

private:
  std::uint64_t state;
};