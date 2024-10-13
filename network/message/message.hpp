#ifndef LIBFTPP_MESSAGE_HPP
# define LIBFTPP_MESSAGE_HPP

# include "../../data_structures/data_buffer/data_buffer.hpp"
# include <stdexcept>

class Message {
public :
	using Type = int;

	explicit Message(Type type);

	Type	type();

	template <typename TType>
	friend Message&	operator<<(Message& msg, const TType& obj);

	template <typename TType>
	friend Message&	operator>>(Message& msg, TType& obj);

	std::string	serialize() const;
	void		deserialize(const std::string& data);

	/* Exceptions */
	class DeserializationFailedException : public std::runtime_error {
	public :
		explicit DeserializationFailedException(const std::string& msg)
		: runtime_error("Message: " + msg + ".") {}
	};

private :
	Type		msgType;
	DataBuffer	data;
};

# include "message.tpp"

#endif