#include <cstdint>

#define MCG59_C     302875106592253
#define MCG59_DEC_M 576460752303423487

struct MCG59
{
	std::uint64_t value;
	std::uint64_t offset;
};

void InitMCG59(MCG59* randomGenerator, std::uint64_t seed, std::uint32_t id, std::uint32_t step);
inline std::uint64_t NextMCG59(MCG59* randomGenerator)
{
	randomGenerator->value = (randomGenerator->value * randomGenerator->offset) & MCG59_DEC_M;
	return randomGenerator->value;
}
