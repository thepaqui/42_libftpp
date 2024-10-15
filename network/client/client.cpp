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

	int flags = fcntl(sockfd, F_GETFL, 0);
	if (flags == -1 || fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1) {
		close(sockfd);
		throw ConnectionFailedException("Failed to set non-blocking mode");
	}

	sockaddr_in	servAddr{};
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(static_cast<uint16_t>(port));

	if (address == "localhost") {
		struct addrinfo	hints{}, *res;
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;

		int	status = getaddrinfo("localhost", nullptr, &hints, &res);
		if (status != 0) {
			close(sockfd);
			throw ConnectionFailedException("Failed to resolve localhost");
		}

		servAddr.sin_addr = reinterpret_cast<sockaddr_in*>(res->ai_addr)->sin_addr;
		freeaddrinfo(res);
	} else if (inet_pton(AF_INET, address.c_str(), &servAddr.sin_addr) <= 0) {
		close(sockfd);
		throw ConnectionFailedException("Invalid address");
	}

	if (::connect(sockfd, reinterpret_cast<sockaddr*>(&servAddr), sizeof(servAddr)) < 0) {
		if (errno != EINPROGRESS) {
			close(sockfd);
			throw ConnectionFailedException("Failed to connect");
		}
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
	enum State {
		NOSIZE,
		SIZE,
		MESSAGE
	};
	State		state = NOSIZE;
	size_t		size;
	std::string	data;
	ssize_t		bytesRead;
	size_t		totalBytes = 0;

	while (isConnected) {
		if (state == NOSIZE) {
			bytesRead = recv(
				sockfd,
				&size + totalBytes,
				sizeof(size) - totalBytes,
				0
			);

			if (bytesRead == -1 && (errno == EWOULDBLOCK || errno == EAGAIN)) {
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				continue ;
			} else if (bytesRead <= 0) {
				shouldEnd = true;
				break ;
			}

			totalBytes += bytesRead;
			if (totalBytes < sizeof(size))
				continue ;

			state = SIZE;
			totalBytes = 0;
			data.resize(size, '\0');
		}

		if (state == SIZE) {
			bytesRead = recv(
				sockfd,
				data.data() + totalBytes,
				size - totalBytes,
				0
			);

			if (bytesRead == -1 && (errno == EWOULDBLOCK || errno == EAGAIN)) {
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				continue ;
			} else if (bytesRead <= 0) {
				shouldEnd = true;
				break ;
			}

			totalBytes += bytesRead;
			if (totalBytes < size)
				continue ;

			state = MESSAGE;
			totalBytes = 0;
		}

		if (state == MESSAGE) {
			state = NOSIZE;
			Message	msg(0);
			try {
				msg.deserialize(data);
			} catch (const std::exception& e) {
				continue ;
			}

			std::lock_guard<std::mutex>	lock(mtx);
			msgs.push_back(std::move(msg));
		}
	}
}
