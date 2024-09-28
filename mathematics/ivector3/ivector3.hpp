#ifndef LIBFTPP_IVECTOR3_HPP
# define LIBFTPP_IVECTOR3_HPP

# include <cmath>

template <typename TType>
struct IVector3 {
	TType	x;
	TType	y;
	TType	z;

	float			length() const;
	IVector3<float>	normalize() const;
	float			dot(const IVector3& vector) const;
	IVector3		cross(const IVector3& vector) const;

	// Operator overloads for + - * / == !=

	IVector3	operator+(const IVector3& obj) const;
	IVector3	operator-(const IVector3& obj) const;
//	IVector3	operator*(const IVector3& obj) const;
//	IVector3	operator/(const IVector3& obj) const;

	bool	operator==(const IVector3& obj) const;
	bool	operator!=(const IVector3& obj) const;
};

# include "ivector3.tpp"

#endif