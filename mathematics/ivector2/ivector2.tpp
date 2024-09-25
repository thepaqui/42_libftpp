#ifndef LIBFTPP_IVECTOR2_TPP
# define LIBFTPP_IVECTOR2_TPP

# include "ivector2.hpp"

template <typename TType>
float
IVector2<TType>::length()
{
	return std::sqrt((x * x) + (y * y));
}

template <typename TType>
IVector2<float>
IVector2<TType>::normalize()
{
	float	magnitude = length();

	IVector2<float>	result;
	result.x = x / magnitude;
	result.y = y / magnitude;

	return result;
}

template <typename TType>
float
IVector2<TType>::dot(const IVector2<TType>& vector)
{
	return (x * vector.x) + (y * vector.y);
}

/*
template <typename TType>
IVector2<TType>
IVector2<TType>::cross(const IVector2<TType>& vector)
{
	
}
*/

#endif