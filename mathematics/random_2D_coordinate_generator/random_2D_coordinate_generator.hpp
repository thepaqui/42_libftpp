#ifndef LIBFTPP_RANDOM_2D_COORDINATE_GENERATOR_HPP
# define LIBFTPP_RANDOM_2D_COORDINATE_GENERATOR_HPP

# include <random>

class Random2DCoordinateGenerator {
private :
	long long	mySeed;

public :
	Random2DCoordinateGenerator();
	Random2DCoordinateGenerator(const long long& newSeed);

	long long	seed() const noexcept;
	void		setSeed(const long long& newSeed) noexcept;
	long long	operator()(const long long& x, const long long& y);
};

#endif