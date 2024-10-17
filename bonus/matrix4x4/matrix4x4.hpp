#ifndef LIBFTPP_MATRIX4X4_HPP
# define LIBFTPP_MATRIX4X4_HPP

//# include "../../mathematics/ivector3/ivector3.hpp"

# include <vector>
# include <initializer_list>
# include <stdexcept>
# include <type_traits> // TODO: Try without
# include <cmath> // TODO: Try without

template <typename TType>
class Matrix4x4
{
private	:
	std::vector<TType>	_data;

	size_t	index(const uint8_t row, const uint8_t col) const noexcept;

public	:
	enum class Type
	{
		MAT_NULL,	// Fills with 0
		MAT_ID		// Identity matrix
	};

	Matrix4x4(const Type type = MAT_NULL);
	Matrix4x4(const Matrix4x4 &obj);
	Matrix4x4(const std::initializer_list<TType> data);
	~Matrix4x4() = default;

	TType*			data() noexcept;
	const TType*	data() const noexcept;

	const TType&	getElem(const size_t row, const size_t col) const;
	void			setElem(const size_t row, const size_t col, const TType n);

	Matrix4x4	&operator=(const Matrix4x4 &obj);
	Matrix4x4	operator+(const Matrix4x4 &obj) const;
	Matrix4x4	operator-(const Matrix4x4 &obj) const;
	Matrix4x4	operator*(const TType n) const;
	Matrix4x4	operator*(const Matrix4x4 &obj) const;
	template <typename std::enable_if<std::is_floating_point<TType>::value, int>::type = 0>
	Matrix4x4	operator/(const TType n) const;
	bool		operator==(const Matrix4x4& obj) const;

	Matrix4x4	transpose(const Matrix4x4 &obj);

	/* Exceptions */

	class InvalidMatrixTypeException : public std::invalid_argument {
	public :
		explicit InvalidMatrixTypeException()
		: invalid_argument("Matrix4x4: Invalid type") {}
	};

	class InvalidInitListException : public std::invalid_argument {
	public :
		explicit InvalidInitListException()
		: invalid_argument("Matrix4x4: Invalid initializer list") {}
	};

	class BadReadException : public std::out_of_range {
	public :
		explicit BadReadException()
		: out_of_range("Matrix4x4: Read access out of range") {}
	};

	class BadWriteException : public std::out_of_range {
	public :
		explicit BadWriteException()
		: out_of_range("Matrix4x4: Write access out of range") {}
	};
};

template <typename TType>
Matrix4x4<TType>	operator*(const TType n, const Matrix4x4<TType> &obj);

# include "matrix4x4.tpp"

#endif