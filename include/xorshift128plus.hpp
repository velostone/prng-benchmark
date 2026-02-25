#pragma once

#include <cstdint>

#include "splitmix64.hpp"

class XorShift128Plus {
public:
	using result_type = std::uint64_t;

	explicit XorShift128Plus(std::uint64_t seed) {
		SplitMix64 sm(seed);
		state[0] = sm.next();
		state[1] = sm.next();
	}

	inline std::uint64_t operator()() {
		std::uint64_t s1 = state[0];
		const std::uint64_t s0 = state[1];
		state[0] = s0;
		s1 ^= s1 << 23; // a
		state[1] = s1 ^ s0 ^ (s1 >> 17) ^ (s0 >> 26); // b, c
		return state[1] + s0;
	}

private:
	std::uint64_t state[2];
};