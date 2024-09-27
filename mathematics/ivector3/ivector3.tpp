#ifndef LIBFTPP_IVECTOR3_TPP
# define LIBFTPP_IVECTOR3_TPP

# include "ivector3.hpp"

template <typename TType>
float
IVector3<TType>::length()
{
	return std::sqrt((x * x) + (y * y) + (z * z));
}

template <typename TType>
IVector3<float>
IVector3<TType>::normalize()
{
	float	magnitude = length();

	IVector3<float>	result;
	result.x = x / magnitude;
	result.y = y / magnitude;
	result.z = z / magnitude;

	return result;
}

template <typename TType>
float
IVector3<TType>::dot(
	const IVector3<TType>& vector
)
{
	return (x * vector.x) + (y * vector.y) + (z * vector.z);
}

template <typename TType>
IVector3<TType>
IVector3<TType>::cross(const IVector3<TType>& vector)
{
	IVector3<TType>	result;
	result.x = (y * vector.z) - (z * vector.y);
	result.y = (z * vector.x) - (x * vector.z);
	result.z = (x * vector.y) - (y * vector.x);

	return result;
}

template <typename TType>
IVector3<TType>
IVector3<TType>::operator+(
	const IVector3& obj
) const
{
	IVector3<TType>	result;
	result.x = x + obj.x;
	result.y = y + obj.y;
	result.z = z + obj.z;

	return result;
}

template <typename TType>
IVector3<TType>
IVector3<TType>::operator-(
	const IVector3& obj
) const
{
	IVector3<TType>	result;
	result.x = x - obj.x;
	result.y = y - obj.y;
	result.z = z - obj.z;

	return result;
}

/*
template <typename TType>
IVector3
IVector3<TType>::operator*(
	const IVector3& obj
) const
{

}
*/

/*
template <typename TType>
IVector3
IVector3<TType>::operator/(
	const IVector3& obj
) const
{

}
*/

template <typename TType>
bool
IVector3<TType>::operator==(
	const IVector3& obj
) const
{
	return (x == obj.x && y == obj.y && z == obj.z);
}

template <typename TType>
bool
IVector3<TType>::operator!=(
	const IVector3& obj
) const
{
	return (x != obj.x || y != obj.y || z != obj.z);
}

#endif