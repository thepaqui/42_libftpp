#include "message.hpp"

Message::Message(
	int type
)
: msgType(type)
{}

int
Message::type()
{
	return msgType;
}
