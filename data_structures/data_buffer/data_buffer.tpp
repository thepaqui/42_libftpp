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

void
DataBuffer::serialize(
	const std::string& str
)
{
	size_t	size = str.size();
	serialize(size);

	const std::byte*	bytes = reinterpret_cast<const std::byte*>(str.data());
	const std::byte*	bytesEnd = reinterpret_cast<const std::byte*>(str.data() + size);
	buffer.insert(buffer.end(), bytes, bytesEnd);
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

void
DataBuffer::deserialize(
	std::string& str
)
{
	size_t	size;
	deserialize(size);

	if (buffer.size() < size)
		throw NotEnoughBytesToDeserializeException();

	const char*	bytesAsStr = reinterpret_cast<const char*>(buffer.data());
	str.assign(bytesAsStr, size);
	buffer.erase(buffer.begin(), buffer.begin() + size);
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