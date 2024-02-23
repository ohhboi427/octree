#pragma once

#include <cstdint>

namespace ob
{
	/**
	 * @brief A constexpr unsigned integer power function.
	 * 
	 * @param base The base of the power.
	 * @param exponent The exponent of the power.
	 * 
	 * @return The result of the power.
	 */
	constexpr auto pow_ce(uint64_t base, uint64_t exponent) -> uint64_t
	{
		uint64_t result = 1u;

		for(uint64_t i = 0u; i < exponent; ++i)
		{
			result *= base;
		}

		return result;
	}
}
