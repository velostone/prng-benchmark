#pragma once

#include <cstdint>

#include "splitmix64.hpp"

static inline std::uint32_t rotl(const std::uint32_t x, int k) {
	return (x << k) | (x >> (32 - k));
}

class XoShiRo128Plus {
public:
	explicit XoShiRo128Plus(std::uint64_t seed) {
		SplitMix64 sm(seed);
		s[0] = (sm.next() >> 32);
		s[1] = (sm.next() >> 32);
		s[2] = (sm.next() >> 32);
		s[3] = (sm.next() >> 32);
	}

	inline std::uint32_t operator()() {
		const std::uint32_t result = s[0] + s[3];
		const std::uint32_t t = s[1] << 9;
		s[2] ^= s[0];
		s[3] ^= s[1];
		s[1] ^= s[2];
		s[0] ^= s[3];
		s[2] ^= t;
		s[3] = rotl(s[3], 11);
		return result;
	}

private:
	std::uint32_t s[4];
};