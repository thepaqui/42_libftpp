#ifndef LIBFTPP_DATA_BUFFER_HPP
# define LIBFTPP_DATA_BUFFER_HPP

# include <vector>
# include <type_traits>
# include <cstddef>
# include <cstring>
# include <string>
# include <stdexcept>

class DataBuffer {
private :
	std::vector<std::byte>	buffer;

	template <typename TType>
	std::enable_if_t<std::is_trivially_copyable_v<TType>, void>	serialize(const TType& obj);

	void	serialize(const std::string& str);

	template <typename TType>
	std::enable_if_t<std::is_trivially_copyable_v<TType>, void>	deserialize(TType& obj);

	void	deserialize(std::string& str);

public :
	template <typename TType>
	friend DataBuffer&	operator<<(DataBuffer& dataBuffer, const TType& obj);

	template <typename TType>
	friend DataBuffer&	operator>>(DataBuffer& dataBuffer, TType& obj);

	/* Exceptions */
	class NotEnoughBytesToDeserializeException : public std::runtime_error {
	public :
		explicit NotEnoughBytesToDeserializeException()
		: runtime_error("DataBuffer: Not enough bytes to deserialize.") {}
	};
};

# include "data_buffer.tpp"

#endif