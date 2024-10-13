# include "data_buffer.hpp"

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

std::vector<std::byte>&
DataBuffer::data() noexcept
{
	return buffer;
}

const std::vector<std::byte>&
DataBuffer::data() const noexcept
{
	return buffer;
}
