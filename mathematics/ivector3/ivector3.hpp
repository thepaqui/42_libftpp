#ifndef LIBFTPP_IVECTOR3_HPP
# define LIBFTPP_IVECTOR3_HPP

# include <cmath>

template <typename TType>
struct IVector3 {
	TType	x;
	TType	y;
	TType	z;

	float						length();
	IVector3<float>	normalize();
	float						dot(const IVector3& vector);
	IVector3				cross(const IVector3& vector);

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