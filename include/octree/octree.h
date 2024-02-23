#pragma once

#include "math/bit.h"
#include "math/pow.h"

#include <vector>

#include <cstdint>

namespace ob
{
	/**
	 * @brief A sparse octree of bytes.
	 * 
	 * @tparam L The number of levels of the tree.
	 */
	template<size_t L>
	class octree
	{
	public:
		/**
		 * @brief The edge size of the octree.
		 *
		 * Equals to 2^@ref L
		 */
		static constexpr size_t size = pow_ce(2u, L);

		/**
		 * @brief Retrieves a value from the octree.
		 *
		 * Will return 0 if the value is not stored in the tree.
		 * 
		 * @param x The x position of the coordinate.
		 * @param y The y position of the coordinate.
		 * @param z The z position of the coordinate.
		 * 
		 * @return A value at the given coordinate.
		 */
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

		/**
		 * @brief Sets a value in the octree.
		 *
		 * Can't be set to 0.
		 * 
		 * @param x The x position of the coordinate.
		 * @param y The y position of the coordinate.
		 * @param z The z position of the coordinate.
		 * @param value The new value.
		 */
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
