#ifndef LIBFTPP_IVECTOR2_HPP
# define LIBFTPP_IVECTOR2_HPP

# include <cmath>

template <typename TType>
struct IVector2 {
	TType	x;
	TType	y;

	float						length();
	IVector2<float>	normalize();
	float						dot(const IVector2& vector);
//	IVector2				cross(const IVector2& vector);

	// Operator overloads for + - * / == !=

	IVector2	operator+(const IVector2& obj) const;
	IVector2	operator-(const IVector2& obj) const;
//	IVector2	operator*(const IVector2& obj) const;
//	IVector2	operator/(const IVector2& obj) const;

	bool	operator==(const IVector2& obj) const;
	bool	operator!=(const IVector2& obj) const;
};

# include "ivector2.tpp"

#endif