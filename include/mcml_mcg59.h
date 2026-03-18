#ifndef __MCML_MCG59_H
#define __MCML_MCG59_H

#include <cstdint>

struct MCG59
{
	std::uint64_t value;
	std::uint64_t offset;
};

//Public
void InitMCG59(MCG59* randomGenerator, std::uint64_t seed, std::uint32_t id, std::uint32_t step);
double NextMCG59(MCG59* randomGenerator);

#endif //__MCML_MCG59_H
