#include "client.hpp"

Client::Client()
: sockfd(-1), isConnected(false)
{}

Client::~Client()
{
	disconnect();
}

void
Client::connect(
	const std::string& address,
	const size_t& port
)
{
	if (isConnected)
		throw AlreadyConnectedException();

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		throw ConnectionFailedException("Failed to create socket");

	sockaddr_in	servAddr{};
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(static_cast<uint16_t>(port));

	if (inet_pton(AF_INET, address.c_str(), &servAddr.sin_addr) <= 0) {
		close(sockfd);
		throw ConnectionFailedException("Invalid address");
	}

	if (::connect(sockfd, reinterpret_cast<sockaddr*>(&servAddr), sizeof(servAddr)) < 0) {
		close(sockfd);
		throw ConnectionFailedException("Failed to connect");
	}

	isConnected = true;
	receiver = std::thread(&Client::receiveMsgs, this);
}

void
Client::disconnect()
{
	if (!isConnected)
		return ;

	isConnected = false;
	if (receiver.joinable())
		receiver.join();

	close(sockfd);
	sockfd = -1;
}

void
Client::send(
	const Message& message
)
{
	if (!isConnected)
		throw NotConnectedException();

	std::string	data = message.serialize();
	size_t		size = data.size();

	if (::send(sockfd, &size, sizeof(size), 0) < 0)
		throw SendingFailedException();

	if (::send(sockfd, data.c_str(), size, 0) < 0)
		throw SendingFailedException();
}

void
Client::defineAction(
	const Message::Type& messageType,
	const Action& action
)
{
	std::lock_guard<std::mutex>	lock(mtx);
	actions[messageType] = action;
}

void
Client::update()
{
	if (!isConnected)
		throw NotConnectedException();

	if (shouldEnd) {
		disconnect();
		return ;
	}

	std::vector<Message>	processingList;

	{
		std::lock_guard<std::mutex>	lock(mtx);
		processingList.swap(msgs);
	}

	for (Message& message : processingList) {
		auto	it = actions.find(message.type());

		if (it != actions.end())
			it->second(message);
	}
}

void
Client::receiveMsgs()
{
	ssize_t	bytesRead;
	while (isConnected) {
		size_t	size;
		bytesRead = recv(sockfd, &size, sizeof(size), 0);
		if (bytesRead <= 0) {
			shouldEnd = true;
			break ;
		}

		std::string	data(size, '\0');
		bytesRead = recv(sockfd, data.data(), size, 0);
		if (bytesRead <= 0) {
			shouldEnd = true;
			break ;
		}

		Message	msg(0);
		msg.deserialize(data);

		std::lock_guard<std::mutex>	lock(mtx);
		msgs.push_back(std::move(msg));
	}
}
