#pragma once

#include <cstdint>

template<typename RNG>
double nextDouble(RNG& gen) {
  std::uint64_t mantissa = gen() & ((UINT64_C(1) << 53) - 1);
  return double(mantissa) / (UINT64_C(1) << 53);
}

template<typename RNG>
float nextFloat(RNG& gen, std::uint8_t shift) {
  std::uint32_t mantissa = (gen() >> shift) & ((UINT32_C(1) << 24) - 1);
  return float(mantissa) / (UINT32_C(1) << 24);
}