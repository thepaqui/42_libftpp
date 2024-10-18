#ifndef LIBFTPP_MATRIX4X4_HPP
# define LIBFTPP_MATRIX4X4_HPP

# include <cstdint>
# include <vector>
# include <initializer_list>
# include <stdexcept>
# include <ostream>
# include <type_traits>

enum class MatrixType
{
	MAT_NULL,	// Fills with 0
	MAT_ID		// Identity matrix
};

template <typename TType>
class Matrix4x4
{
static_assert(
	std::is_arithmetic_v<TType>,
	"Matrix4x4 class can only be instantiated with arithmetic types."
);

private :
	std::vector<TType>	_data;

	size_t	index(const uint8_t row, const uint8_t col) const noexcept;

public :
	Matrix4x4(MatrixType type = MatrixType::MAT_NULL);
	Matrix4x4(const Matrix4x4 &obj);
	Matrix4x4(const std::initializer_list<TType> data);
	~Matrix4x4() = default;

	TType*			data() noexcept;
	const TType*	data() const noexcept;

	const TType&	getElem(const size_t row, const size_t col) const;
	void			setElem(const size_t row, const size_t col, const TType n);

	Matrix4x4&			operator=(const Matrix4x4 &obj);
	Matrix4x4			operator+(const Matrix4x4 &obj) const;
	Matrix4x4			operator-(const Matrix4x4 &obj) const;
	Matrix4x4			operator*(const TType n) const;
	Matrix4x4			operator*(const Matrix4x4 &obj) const;
	Matrix4x4<float>	operator/(const TType n) const;
	template <typename UType = TType, typename std::enable_if<std::is_integral<UType>::value, int>::type = 0>
	Matrix4x4			operator%(const TType n) const;
	bool				operator==(const Matrix4x4& obj) const;
	bool				operator!=(const Matrix4x4& obj) const;

	Matrix4x4	transpose() const;

	static Matrix4x4<float>	average(const Matrix4x4& obj1, const Matrix4x4& obj2);
	static Matrix4x4		compMult(const Matrix4x4& obj1, const Matrix4x4& obj2);

	/* Exceptions */

	class InvalidMatrixTypeException : public std::invalid_argument {
	public :
		explicit InvalidMatrixTypeException()
		: invalid_argument("Matrix4x4: Invalid matrix type") {}
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

template <typename TType>
std::ostream&	operator<<(std::ostream &ostream, const Matrix4x4<TType> &obj);

# include "matrix4x4.tpp"

#endif