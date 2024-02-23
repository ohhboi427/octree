#include <octree/octree.h>

#include <chrono>
#include <iostream>

template<typename T>
constexpr auto max(T a, T b) -> T
{
	return (a > b) ? a : b;
}

template<uint8_t N>
auto GenerateVoxelData(ob::octree<N>& chunk) -> void
{
	for(uint8_t x = 0u; x < ob::octree<N>::size; x++)
	{
		for(uint8_t y = 0u; y < ob::octree<N>::size; y++)
		{
			for(uint8_t z = 0u; z < ob::octree<N>::size; z++)
			{
				chunk.set(x, y, z, z);
			}
		}
	}
}

auto main(int argc, char* argv[]) -> int
{
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

	ob::octree<2u> octree;
	GenerateVoxelData(octree);

	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();

	uint8_t n = octree.get(0u, 31, 13);

	std::cout << static_cast<int>(n) << "    " << std::chrono::duration_cast<std::chrono::duration<double>>(end - start) << std::endl;
}
