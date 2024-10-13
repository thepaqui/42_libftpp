#ifndef LIBFTPP_MESSAGE_TPP
# define LIBFTPP_MESSAGE_TPP

# include "message.hpp"

template <typename TType>
Message&
operator<<(
	Message& msg,
	const TType& obj
)
{
	msg.data << obj;
	return msg;
}

template <typename TType>
Message&
operator>>(
	Message& msg,
	TType& obj
)
{
	msg.data >> obj;
	return msg;
}

#endif