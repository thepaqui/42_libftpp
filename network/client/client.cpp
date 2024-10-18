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

		fd_set	writefds;
		FD_ZERO(&writefds);
		FD_SET(sockfd, &writefds);

		struct timeval timeout;
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;

		int	selectResult = select(sockfd + 1, nullptr, &writefds, nullptr, &timeout);
		if (selectResult == 0) {
			close(sockfd);
			throw ConnectionFailedException("Failed to connect (timeout)");
		} else if (selectResult < 0 || !FD_ISSET(sockfd, &writefds)) {
			close(sockfd);
			throw ConnectionFailedException("Failed to connect (select error)");
		}

		int			error = 0;
		socklen_t	len = sizeof(error);
		if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len) < 0
		|| error != 0) {
			close(sockfd);
			throw ConnectionFailedException("Failed to connect: " + std::string(strerror(error)));
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

	std::lock_guard<std::mutex>	lock(mtx);
	msgsToSend.push(message);
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
		throw NotConnectedException();
	}

	std::vector<Message>						processingList;
	std::unordered_map<Message::Type, Action>	actionsList;

	{
		std::lock_guard<std::mutex>	lock(mtx);
		processingList.swap(msgs);
		actionsList = actions;
	}

	for (Message& msg : processingList) {
		auto	it = actionsList.find(msg.type());

		if (it != actionsList.end() && it->second)
			it->second(msg);
	}
}

void
Client::receiveMsg(ClientBuf& clientBuf)
{
	if (clientBuf.state == ClientBuf::NOSIZE) {
		clientBuf.bytesRead = recv(
			sockfd,
			&clientBuf.size + clientBuf.totalBytes,
			sizeof(clientBuf.size) - clientBuf.totalBytes,
			0
		);

		if (clientBuf.bytesRead <= 0) {
			shouldEnd = true;
			return ;
		}

		clientBuf.totalBytes += clientBuf.bytesRead;
		if (clientBuf.totalBytes < sizeof(clientBuf.size))
			return ;

		clientBuf.state = ClientBuf::SIZE;
		clientBuf.totalBytes = 0;
		clientBuf.data.resize(clientBuf.size, '\0');

	} else if (clientBuf.state == ClientBuf::SIZE) {
		clientBuf.bytesRead = recv(
			sockfd,
			clientBuf.data.data() + clientBuf.totalBytes,
			clientBuf.size - clientBuf.totalBytes,
			0
		);

		if (clientBuf.bytesRead <= 0) {
			shouldEnd = true;
			return ;
		}

		clientBuf.totalBytes += clientBuf.bytesRead;
		if (clientBuf.totalBytes < clientBuf.size)
			return ;

		clientBuf.state = ClientBuf::MESSAGE;
		clientBuf.totalBytes = 0;
	}

	if (clientBuf.state == ClientBuf::MESSAGE) {
		clientBuf.state = ClientBuf::NOSIZE;
		Message	msg(0);
		try {
			msg.deserialize(clientBuf.data);
		} catch (const std::exception& e) {
			return ;
		}

		std::lock_guard<std::mutex>	lock(mtx);
		msgs.push_back(std::move(msg));
	}
}

void
Client::sendMsg(
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
Client::receiveMsgs()
{
	ClientBuf	clientBuf;

	fd_set	readfds;
	fd_set	writefds;

	while (isConnected) {
		FD_ZERO(&readfds);
		FD_ZERO(&writefds);
		FD_SET(sockfd, &readfds);
		FD_SET(sockfd, &writefds);
		int	max_fd = sockfd;

		int	activity = select(max_fd + 1, &readfds, &writefds, nullptr, nullptr);
		if (activity < 0)
			continue ;

		if (!shouldEnd && FD_ISSET(sockfd, &readfds)) {
			receiveMsg(clientBuf);
		}

		if (!shouldEnd && FD_ISSET(sockfd, &writefds)) {
			Message	msg(0);
			bool	send = false;
			{
				std::lock_guard<std::mutex>	lock(mtx);
				if (!msgsToSend.empty()) {
					msg = msgsToSend.front();
					msgsToSend.pop();
					send = true;
				}
			}
			if (send)
				sendMsg(msg);
		}
	}
}
