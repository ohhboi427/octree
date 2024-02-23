#pragma once

#include <bit>

#include <cstdint>

namespace ob
{
	/**
	 * @brief Counts the number of set bits in a byte.
	 *
	 * @param byte The byte.
	 * @param offset The offset of the starting bit.
	 * @param count The number of counted bits.
	 * 
	 * @return The number of set bits.
	 */
	constexpr auto pop_count_byte(uint8_t byte, uint8_t offset = 0u, uint8_t count = 8u) -> size_t
	{
		uint8_t mask = ~(~0u << count) << offset;
		byte &= mask;

		return std::popcount(byte);
	}

	/**
	 * @brief Counts the set bits in multiple bytes.
	 * 
	 * @param begin A pointer to the first byte.
	 * @param end A pointer after the last byte.
	 * 
	 * @return The number of set bits.
	 */
	constexpr auto pop_count_range(const uint8_t* begin, const uint8_t* end) -> size_t
	{
		uint64_t sum = 0u;

		while(begin < end)
		{
			sum += std::popcount(*begin);

			++begin;
		}

		return sum;
	}
}
