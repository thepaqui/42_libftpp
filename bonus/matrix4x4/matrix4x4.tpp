#ifndef LIBFTPP_MATRIX4X4_TPP
# define LIBFTPP_MATRIX4X4_TPP

# include "matrix4x4.hpp"

// Constructors

template <typename TType>
Matrix4x4<TType>::Matrix4x4(
	MatrixType type
)
: _data(16, 0)
{
	if (type != MatrixType::MAT_NULL && type != MatrixType::MAT_ID)
		throw InvalidMatrixTypeException();

	if (type == MatrixType::MAT_ID)
		for (size_t i = 0; i < 4; i++)
			_data[index(i, i)] = 1;
}

template <typename TType>
Matrix4x4<TType>::Matrix4x4(
	const Matrix4x4<TType> &obj
)
: _data(obj._data)
{}

template <typename TType>
Matrix4x4<TType>::Matrix4x4(
	const std::initializer_list<TType> data
)
{
	if (data.size() != 16)
		throw InvalidInitListException();

	_data.assign(data);
}

// Methods

template <typename TType>
size_t
Matrix4x4<TType>::index(
	const uint8_t row,
	const uint8_t col
) const noexcept
{
	return (row * 4 + col);
}

template <typename TType>
TType*
Matrix4x4<TType>::data() noexcept
{
	return _data.data();
}

template <typename TType>
const TType*
Matrix4x4<TType>::data() const noexcept
{
	return _data.data();
}

template <typename TType>
const TType&
Matrix4x4<TType>::getElem(
	const size_t row,
	const size_t col
) const
{
	if (row >= 4 || col >= 4)
		throw BadReadException();
	return _data[index(row, col)];
}

template <typename TType>
void
Matrix4x4<TType>::setElem(
	const size_t row,
	const size_t col,
	const TType n
)
{
	if (row >= 4 || col >= 4)
		throw BadWriteException();
	_data[index(row, col)] = n;
}

// Operator overloads

template <typename TType>
Matrix4x4<TType>&
Matrix4x4<TType>::operator=(
	const Matrix4x4<TType> &obj
)
{
	if (this != &obj)
		_data = obj._data;

	return (*this);
}

template <typename TType>
Matrix4x4<TType>
Matrix4x4<TType>::operator+(
	const Matrix4x4<TType> &obj
) const
{
	Matrix4x4<TType>	ret(*this);

	for (size_t i = 0; i < 16; i++)
		ret._data[i] += obj._data[i];

	return ret;
}

template <typename TType>
Matrix4x4<TType>
Matrix4x4<TType>::operator-(
	const Matrix4x4<TType> &obj
) const
{
	Matrix4x4<TType>	ret(*this);

	for (size_t i = 0; i < 16; i++)
		ret._data[i] -= obj._data[i];

	return ret;
}

template <typename TType>
Matrix4x4<TType>
Matrix4x4<TType>::operator*(
	const TType n
) const
{
	Matrix4x4<TType>	ret(*this);

	for (size_t i = 0; i < 16; i++)
		ret._data[i] *= n;

	return ret;
}

template <typename TType>
Matrix4x4<TType>
Matrix4x4<TType>::operator*(
	const Matrix4x4 &obj
) const
{
	Matrix4x4<TType>	ret;

	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			TType	val = 0;
			for (size_t k = 0; k < 4; k++)
				val += getElem(i, k) * obj.getElem(k, j);
			ret.setElem(i, j, val);
		}
	}

	return ret;
}

template <typename TType>
Matrix4x4<float>
Matrix4x4<TType>::operator/(
	const TType n
) const
{
	Matrix4x4<float>	ret;

	for (size_t i = 0; i < 16; i++)
		ret.data()[i] = static_cast<float>(_data[i]) / static_cast<float>(n);

	return ret;
}

template <typename TType>
template <typename UType, typename std::enable_if<std::is_integral<UType>::value, int>::type>
Matrix4x4<TType>
Matrix4x4<TType>::operator%(
	const TType n
) const
{
	Matrix4x4<TType>	ret(*this);

	for (size_t i = 0; i < 16; i++)
		ret._data[i] %= n;

	return ret;
}

template <typename TType>
Matrix4x4<TType>
operator*(
	const TType n,
	const Matrix4x4<TType> &obj
)
{
	Matrix4x4<TType>	ret(obj);

	for (size_t i = 0; i < 16; i++)
		ret.data()[i] *= n;

	return ret;
}

template<typename TType>
bool
Matrix4x4<TType>::operator==(
	const Matrix4x4& obj
) const
{
	for (size_t i = 0; i < 16; i++)
		if (_data[i] != obj._data[i])
			return false;

	return true;
}

template<typename TType>
bool
Matrix4x4<TType>::operator!=(
	const Matrix4x4& obj
) const
{
	for (size_t i = 0; i < 16; i++)
		if (_data[i] != obj._data[i])
			return true;

	return false;
}

// Others

template <typename TType>
Matrix4x4<TType>
Matrix4x4<TType>::transpose() const
{
	Matrix4x4	ret;

	for (size_t i = 0; i < 4; i++)
		for (size_t j = 0; j < 4; j++)
			ret.setElem(j, i, this->getElem(i, j));

	return ret;
}

template <typename TType>
Matrix4x4<float>
Matrix4x4<TType>::average(
	const Matrix4x4 &obj1,
	const Matrix4x4 &obj2
)
{
	return Matrix4x4<float>((obj1 + obj2) / 2);
}

template <typename TType>
Matrix4x4<TType>
Matrix4x4<TType>::compMult(
	const Matrix4x4 &obj1,
	const Matrix4x4 &obj2
)
{
	Matrix4x4	ret(obj1);

	for (size_t i = 0; i < 16; i++)
		ret.data()[i] *= obj2.data()[i];

	return ret;
}

template <typename TType>
std::ostream&
operator<<(
	std::ostream &ostream,
	const Matrix4x4<TType> &obj
)
{
	for (size_t i = 0; i < 4; i++) {
		ostream << "| ";
		for (size_t j = 0; j < 4; j++) {
			ostream << obj.getElem(i, j) << " ";
		}
		ostream << "|\n";
	}
	ostream << std::flush;
	return (ostream);
}

#endif