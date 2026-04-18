#include "mcml_mcg59.h"

std::uint64_t RaiseToPower(std::uint64_t argument, std::uint32_t power)
{
	std::uint64_t result = 1;

	while (power > 0)
	{
		if ((power & 1) == 0)
		{
			argument *= argument;
			power >>= 1;
		}
		else
		{
			result *= argument;
			--power;
		}
	}

	return result;
}

void InitMCG59(MCG59* randomGenerator, std::uint64_t seed, std::uint32_t id, std::uint32_t step)
{
	std::uint64_t value = 2 * seed + 1;
	std::uint64_t firstOffset = RaiseToPower(MCG59_C, id);
	value = (value * firstOffset) & MCG59_DEC_M;
	randomGenerator->value = value;
	randomGenerator->offset = RaiseToPower(MCG59_C, step);
}

