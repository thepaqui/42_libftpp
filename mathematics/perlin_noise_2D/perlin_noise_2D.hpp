#ifndef LIBFTPP_PERLIN_NOISE_2D_HPP
# define LIBFTPP_PERLIN_NOISE_2D_HPP

# include "../ivector2/ivector2.hpp"
# include <vector>
# include <cmath>
# include <cstdlib>
# include <time.h>
# include <utility>

class PerlinNoise2D {
private :
	std::vector<int>	permutationTable;

	unsigned int	randomInt(unsigned int min, unsigned int max);
	void			shufflePermutationTable();
	IVector2<float>	getConstantVector(const int value) const noexcept;
	float			lerp(float t, float min, float max) const noexcept;
	float			fade(float t) const noexcept;

public :
	PerlinNoise2D();
	float	sample(float x, float y);
	float	operator()(float x, float y);
};

#endif