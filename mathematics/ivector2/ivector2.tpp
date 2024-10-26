#ifndef LIBFTPP_IVECTOR2_TPP
# define LIBFTPP_IVECTOR2_TPP

# include "ivector2.hpp"

template <typename TType>
IVector2<TType>::IVector2()
{
	x = 0;
	y = 0;
}

template <typename TType>
IVector2<TType>::IVector2(
	TType xVal,
	TType yVal
)
{
	x = xVal;
	y = yVal;
}

template <typename TType>
float
IVector2<TType>::length() const
{
	return std::sqrt((x * x) + (y * y));
}

template <typename TType>
IVector2<float>
IVector2<TType>::normalize() const
{
	float	magnitude = length();

	IVector2<float>	result;
	result.x = x / magnitude;
	result.y = y / magnitude;

	return result;
}

template <typename TType>
float
IVector2<TType>::dot(
	const IVector2<TType>& vector
) const
{
	return (x * vector.x) + (y * vector.y);
}

/*
template <typename TType>
IVector2<TType>
IVector2<TType>::cross(
	const IVector2<TType>& vector
) const
{
	
}
*/

template <typename TType>
IVector2<TType>
IVector2<TType>::operator+(
	const IVector2& obj
) const
{
	IVector2<TType>	result;
	result.x = x + obj.x;
	result.y = y + obj.y;

	return result;
}

template <typename TType>
IVector2<TType>
IVector2<TType>::operator-(
	const IVector2& obj
) const
{
	IVector2<TType>	result;
	result.x = x - obj.x;
	result.y = y - obj.y;

	return result;
}

/*
template <typename TType>
IVector2
IVector2<TType>::operator*(
	const IVector2& obj
) const
{

}
*/

/*
template <typename TType>
IVector2
IVector2<TType>::operator/(
	const IVector2& obj
) const
{

}
*/

template <typename TType>
bool
IVector2<TType>::operator==(
	const IVector2& obj
) const
{
	return (x == obj.x && y == obj.y);
}

template <typename TType>
bool
IVector2<TType>::operator!=(
	const IVector2& obj
) const
{
	return (x != obj.x || y != obj.y);
}

#endif