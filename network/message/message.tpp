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
	data << obj;
}

template <typename TType>
Message&
operator>>(
	Message& msg,
	TType& obj
)
{
	data >> obj;
}

#endif