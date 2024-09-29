#include "random_2D_coordinate_generator.hpp"

Random2DCoordinateGenerator::Random2DCoordinateGenerator()
{
	mySeed = std::random_device{}();
}

Random2DCoordinateGenerator::Random2DCoordinateGenerator(
	const long long& newSeed
)
: mySeed(newSeed)
{}

long long
Random2DCoordinateGenerator::seed() const noexcept
{
	return mySeed;
}

void
Random2DCoordinateGenerator::setSeed(
	const long long& newSeed
) noexcept
{
	mySeed = newSeed;
}

long long
Random2DCoordinateGenerator::operator()(
	const long long& x,
	const long long& y
)
{
	std::seed_seq	seq{mySeed, x, y};
	std::mt19937_64	generator(seq);

	std::uniform_int_distribution<long long>	distribution(
		0,
		std::numeric_limits<long long>::max()
	);

	return distribution(generator);
}
