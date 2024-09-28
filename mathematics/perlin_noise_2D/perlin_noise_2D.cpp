#include "perlin_noise_2D.hpp"

PerlinNoise2D::PerlinNoise2D()
{
	permutationTable.resize(256);
	std::srand(time(NULL));

	for (int i = 0; i < 256; i++)
		permutationTable[i] = i;

	shufflePermutationTable();
}

void
PerlinNoise2D::shufflePermutationTable()
{
	for (int e = permutationTable.size() - 1; e > 0; e--) {
		const float	index = std::round(std::rand() * (e - 1));
		std::swap(permutationTable[e], permutationTable[index]);
	}
}

IVector2<float>
PerlinNoise2D::getConstantVector(
	const int value
) const noexcept
{
	// '& 3' is equivalent to '% 4'
	const int v = value & 3;

	switch (v) {
		case 0:
			return IVector2<float>(1.0f, 1.0f);
		case 1:
			return IVector2<float>(-1.0f, 1.0f);
		case 2:
			return IVector2<float>(-1.0f, -1.0f);
		default:
			return IVector2<float>(1.0f, -1.0f);
	}
}

float
PerlinNoise2D::lerp(
	float t,
	float min,
	float max
) const noexcept
{
	if (min > max)
		std::swap(min, max);
	return min + t * (max - min);
}

float
PerlinNoise2D::fade(
	float t
) const noexcept
{
	return ((6 * t - 15) * t + 10) * t * t * t;
}

float
PerlinNoise2D::sample(
	float x,
	float y
)
{
	const float	xf = x - std::floor(x);
	const float	yf = y - std::floor(y);
	const IVector2<float>	topRightVector(   xf - 1.0f, yf - 1.0f);
	const IVector2<float>	topLeftVector(    xf,        yf - 1.0f);
	const IVector2<float>	bottomRightVector(xf - 1.0f, yf);
	const IVector2<float>	bottomLeftVector( xf,        yf);

	// '& 255' is equivalent to '% 256'
	const int	xIndex = static_cast<int>(std::floor(x)) & 255;
	const int	yIndex = static_cast<int>(std::floor(y)) & 255;
	const int	topRightValue = permutationTable[permutationTable[xIndex + 1] + yIndex + 1];
	const int	topLeftValue = permutationTable[permutationTable[xIndex] + yIndex + 1];
	const int	bottomRightValue = permutationTable[permutationTable[xIndex + 1] + yIndex];
	const int	bottomLeftValue = permutationTable[permutationTable[xIndex] + yIndex];

	const float	topRightDot = topRightVector.dot(getConstantVector(topRightValue));
	const float	topLeftDot = topLeftVector.dot(getConstantVector(topLeftValue));
	const float	bottomRightDot = bottomRightVector.dot(getConstantVector(bottomRightValue));
	const float	bottomLeftDot = bottomLeftVector.dot(getConstantVector(bottomLeftValue));

	const float	u = fade(xf);
	const float	v = fade(yf);
	return lerp(u,
		lerp(v, bottomLeftDot, topLeftDot),
		lerp(v, bottomRightDot, topRightDot)
	);
}
