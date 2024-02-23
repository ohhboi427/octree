#pragma once

#include <bit>

#include <vector>

#include <cstdint>

namespace ob
{
	constexpr auto pow_ce(uint64_t base, uint64_t exponent) -> uint64_t
	{
		uint64_t result = 1u;

		for(uint64_t i = 0u; i < exponent; ++i)
		{
			result *= base;
		}

		return result;
	}

	constexpr auto pop_count_byte(uint8_t byte, uint8_t offset = 0u, uint8_t count = 8u) -> size_t
	{
		uint8_t mask = ~(~0u << count) << offset;
		byte &= mask;

		return std::popcount(byte);
	}

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

	template<size_t L>
	class octree
	{
	public:
		static constexpr size_t size = pow_ce(2u, L);

		auto get(size_t x, size_t y, size_t z) const noexcept -> uint8_t
		{
			if(m_nodes.empty())
			{
				return 0u;
			}

			size_t head_index = 0u;
			size_t parent_index = 0u;
			uint8_t child_index_in_parent = 8u;

			size_t half = m_half;
			while(half >= 1u)
			{
				uint8_t child_index =
					(x >= half) |
					((y >= half) << 1u) |
					((z >= half) << 1u);

				x -= (x >= half) * half;
				y -= (y >= half) * half;
				z -= (z >= half) * half;

				uint8_t child_mask = 1u << child_index;

				size_t skip_count =
					pop_count_byte(m_nodes[parent_index], child_index_in_parent + 1u) + // The number of children on the same layer after the current.
					pop_count_range(&m_nodes[parent_index] + 1u, &m_nodes[head_index]) + // The number of nodes between the head and parent.
					pop_count_byte(m_nodes[head_index], 0u, child_index) + // The number of children of head.
					1u; // The head.

				child_index_in_parent = child_index;

				if(m_nodes[head_index] & child_mask)
				{
					parent_index = head_index;
					head_index += skip_count;
				}
				else
				{
					return 0u;
				}

				half /= 2u;
			}

			return m_nodes[head_index];
		}

		auto set(size_t x, size_t y, size_t z, uint8_t value) -> void
		{
			if(value == 0u)
			{
				return;
			}

			if(m_nodes.empty())
			{
				m_nodes.emplace_back(0u);
			}

			size_t head_index = 0u;
			size_t parent_index = 0u;
			uint8_t child_index_in_parent = 8u;

			size_t half = m_half;
			while(half >= 1u)
			{
				uint8_t child_index =
					(x >= half) |
					((y >= half) << 1u) |
					((z >= half) << 1u);

				x -= (x >= half) * half;
				y -= (y >= half) * half;
				z -= (z >= half) * half;

				uint8_t child_mask = 1u << child_index;

				uint64_t skip_count =
					pop_count_byte(m_nodes[parent_index], child_index_in_parent + 1u) + // The number of children on the same layer after the current.
					pop_count_range(&m_nodes[parent_index] + 1u, &m_nodes[head_index]) + // The number of nodes between the head and parent.
					pop_count_byte(m_nodes[head_index], 0u, child_index) + // The number of children of head.
					1u; // The head.

				child_index_in_parent = child_index;

				if(m_nodes[head_index] & child_mask)
				{
					parent_index = head_index;
					head_index += skip_count;
				}
				else
				{
					m_nodes[head_index] |= child_mask;

					parent_index = head_index;
					head_index += skip_count;
					auto new_node = m_nodes.insert(m_nodes.begin() + head_index, 0u);
				}

				half /= 2u;
			}

			m_nodes[head_index] = value;
		}

	private:
		static constexpr size_t m_half = size / 2u;

		std::vector<uint8_t> m_nodes;
	};
}
