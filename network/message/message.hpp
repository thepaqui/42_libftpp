#ifndef LIBFTPP_MESSAGE_HPP
# define LIBFTPP_MESSAGE_HPP

# include "../../data_structures/data_buffer/data_buffer.hpp"

class Message {
private :
	int			msgType;
	DataBuffer	data;

public :
	Message(int type);

	int	type();

	template <typename TType>
	friend Message&	operator<<(Message& msg, const TType& obj);

	template <typename TType>
	friend Message&	operator>>(Message& msg, TType& obj);
};

# include "message.tpp"

#endif