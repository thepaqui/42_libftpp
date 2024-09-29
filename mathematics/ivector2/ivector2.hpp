#ifndef LIBFTPP_IVECTOR2_HPP
# define LIBFTPP_IVECTOR2_HPP

# include <cmath>

template <typename TType>
struct IVector2 {
	TType	x;
	TType	y;

	IVector2();
	IVector2(TType xVal, TType yVal);

	float			length() const;
	IVector2<float>	normalize() const;
	float			dot(const IVector2& vector) const;
//	IVector2		cross(const IVector2& vector) const;

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