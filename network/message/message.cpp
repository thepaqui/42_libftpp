#include "message.hpp"

Message::Message(
	Type type
)
: msgType(type)
{}

Message::Type
Message::type()
{
	return msgType;
}

std::string
Message::serialize() const
{
	std::string	result;
	result.resize(sizeof(msgType) + data.data().size());

	std::memcpy(
		result.data(),
		&msgType,
		sizeof(msgType)
	);

	std::memcpy(
		result.data() + sizeof(msgType),
		data.data().data(),
		data.data().size()
	);

	return result;
}

void
Message::deserialize(
	const std::string& str
)
{
	if (str.size() < sizeof(msgType))
		throw DeserializationFailedException("Failed to receive data");

	std::memcpy(
		&msgType,
		str.data(),
		sizeof(msgType)
	);

	data.data().clear();
	data.data().resize(str.size() - sizeof(msgType));

	std::memcpy(
		data.data().data(),
		str.data() + sizeof(msgType),
		str.size() - sizeof(msgType)
	);
}
