#pragma once

#include <cstdint>

#include "splitmix64.hpp"

class XoShiRo256Plus {
public:
	explicit XoShiRo256Plus(std::uint64_t seed) {
		SplitMix64 sm(seed);
		s[0] = sm.next();
		s[1] = sm.next();
		s[2] = sm.next();
		s[3] = sm.next();
	}

	inline std::uint64_t operator()() {
		const std::uint64_t result = s[0] + s[3];
		const std::uint64_t t = s[1] << 17;
		s[2] ^= s[0];
		s[3] ^= s[1];
		s[1] ^= s[2];
		s[0] ^= s[3];
		s[2] ^= t;
		s[3] = rotl(s[3], 45);
		return result;
	}

private:
	std::uint64_t s[4];
};