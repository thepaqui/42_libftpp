#ifndef LIBFTPP_DATA_BUFFER_TPP
# define LIBFTPP_DATA_BUFFER_TPP

# include "data_buffer.hpp"

template <typename TType>
std::enable_if_t<std::is_trivially_copyable_v<TType>, void>
DataBuffer::serialize(
	const TType& obj
)
{
	const std::byte*	bytes = reinterpret_cast<const std::byte*>(&obj);
	buffer.insert(buffer.end(), bytes, bytes + sizeof(TType));
}

template <typename TType>
std::enable_if_t<std::is_trivially_copyable_v<TType>, void>
DataBuffer::deserialize(
	TType& obj
)
{
	if (buffer.size() < sizeof(TType))
		throw NotEnoughBytesToDeserializeException();

	std::memcpy(&obj, buffer.data(), sizeof(TType));
	buffer.erase(buffer.begin(), buffer.begin() + sizeof(TType));
}

template <typename TType>
DataBuffer&
operator<<(
	DataBuffer& dataBuffer,
	const TType& obj
)
{
	dataBuffer.serialize(obj);
	return dataBuffer;
}

template <typename TType>
DataBuffer&
operator>>(
	DataBuffer& dataBuffer,
	TType& obj
)
{
	dataBuffer.deserialize(obj);
	return dataBuffer;
}

#endif