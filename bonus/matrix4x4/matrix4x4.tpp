#ifndef LIBFTPP_MATRIX4X4_TPP
# define LIBFTPP_MATRIX4X4_TPP

# include "matrix4x4.hpp"

// Constructors

template <typename TType>
Matrix4x4<TType>::Matrix4x4(
	const Type type
)
{
	// TODO: Check might now be useless
	if (type != Type::MAT_NULL && type != Type::MAT_ID)
		throw InvalidMatrixTypeException();

	if (type == Type::MAT_ID)
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				if (i == j)
					this->_data.push_back(1);
				else
					this->_data.push_back(0);
			}
		}
	}
	else
		this->_data.assign(16, 0);
}

template <typename TType>
Matrix4x4<TType>::Matrix4x4(
	const Matrix4x4<TType> &obj
)
{
	this->_data = obj._data;
}

template <typename TType>
Matrix4x4<TType>::Matrix4x4(
	const std::initializer_list<TType> data
)
{
	if (data.size() != 16)
		throw InvalidInitListException();

	this->_data.assign(data);
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
	size_t	i = index(row, col);
	if (i >= 16)
		throw BadReadException();
	return this->_data[i];
}

template <typename TType>
void
Matrix4x4<TType>::setElem(
	const size_t row,
	const size_t col,
	const TType n
)
{
	size_t	i = index(row, col);
	if (i >= 16)
		throw BadWriteException();
	this->_data[i] = n;
}

// Operator overloads

template <typename TType>
Matrix4x4<TType>&
Matrix4x4<TType>::operator=(
	const Matrix4x4<TType> &obj
)
{
	if (this == &obj)
		return (*this);

	this->_data = obj._data;
	return (*this);
}

template <typename TType>
Matrix4x4<TType>
Matrix4x4<TType>::operator+(
	const Matrix4x4<TType> &obj
) const
{
	Matrix4x4<TType>	ret(*this);

	for (size_t i = 0; i < 4; i++)
		for (size_t j = 0; j < 4; j++)
			ret._data[index(i, j)] += obj.getElem(i, j);

	return ret;
}

template <typename TType>
Matrix4x4<TType>
Matrix4x4<TType>::operator-(
	const Matrix4x4<TType> &obj
) const
{
	Matrix4x4<TType>	ret(*this);

	for (size_t i = 0; i < 4; i++)
		for (size_t j = 0; j < 4; j++)
			ret._data[index(i, j)] -= obj.getElem(i, j);

	return ret;
}

template <typename TType>
Matrix4x4<TType>
Matrix4x4<TType>::operator*(
	const TType n
) const
{
	Matrix4x4<TType>	ret(*this);

	for (size_t i = 0; i < 4; i++)
		for (size_t j = 0; j < 4; j++)
			ret._data[index(i, j)] *= n;

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
			for (size_t p = 0; p < 4; p++)
				val += this->getElem(i, p) * obj.getElem(p, j);
			ret.setElem(i, j, val);
		}
	}

	return ret;
}

template <typename TType>
template <typename std::enable_if<std::is_floating_point<TType>::value, int>::type>
Matrix4x4<TType>
Matrix4x4<TType>::operator/(
	const TType n
) const
{
	Matrix4x4<TType>	ret(*this);

	for (size_t i = 0; i < 4; i++)
		for (size_t j = 0; j < 4; j++)
			ret._data[index(i, j)] /= n;

	return ret;
}

template <typename TType>
Matrix4x4<TType>
operator*(
	const TType n,
	const Matrix4x4<TType> &obj
)
{
	Matrix4x4<TType>	ret;

	for (size_t i = 0; i < 4; i++)
		for (size_t j = 0; j < 4; j++)
			ret.setElem(i, j, obj.getElem(i, j) * n);

	return ret;
}

// Others

template<typename TType>
bool
Matrix4x4<TType>::operator==(
	const Matrix4x4& obj
) const
{
	for (size_t i = 0; i < 4; i++)
		for (size_t j = 0; j < 4; j++)
			if (this->getElem(i, j) != obj2.getElem(i, j))
				return false;

	return true;
}

template <typename TType>
Matrix4x4<TType>
Matrix4x4<TType>::transpose(
	const Matrix4x4 &obj
)
{
	Matrix4x4	ret;

	for (size_t i = 0; i < 4; i++)
		for (size_t j = 0; j < 4; j++)
			ret.setElem(j, i, obj.getElem(i, j));

	return ret;
}

#endif