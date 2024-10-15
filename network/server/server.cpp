#include "server.hpp"

Server::Server()
: sockfd(-1), isStarted(false)
{}

Server::~Server()
{
	stop();
}

void
Server::start(
	const size_t& p_port
)
{
	if (isStarted)
		throw AlreadyStartedException();
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		throw StartFailedException("Failed to create socket");

	{
		const int	enable = 1;
		setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
		setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int));
	}

	int flags = fcntl(sockfd, F_GETFL, 0);
	if (flags == -1 || fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1) {
		close(sockfd);
		throw StartFailedException("Failed to set non-blocking mode");
	}

	sockaddr_in	servAddr{};
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = INADDR_ANY; // All interfaces
	servAddr.sin_port = htons(static_cast<uint16_t>(p_port));

	if (bind(sockfd, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
		close(sockfd);
		throw StartFailedException("Failed to bind socket");
	}

	if (listen(sockfd, 5) < 0) {
		close(sockfd);
		throw StartFailedException("Failed to listen on socket");
	}

	isStarted = true;
	receiver = std::thread(&Server::receiveMsgs, this);
}

void
Server::stop()
{
	if (!isStarted)
		return ;

	isStarted = false;
	if (receiver.joinable())
		receiver.join();

	for (const auto& [clientID, client] : clients)
		close(client.sockfd);

	close(sockfd);
	sockfd = -1;
}

void
Server::defineAction(
	const Message::Type& messageType,
	const Action& action
)
{
	std::lock_guard<std::mutex>	lock(mtx);
	actions[messageType] = action;
}

void
Server::sendTo(
	const Message& message,
	ClientID clientID
)
{
	if (!isStarted)
		throw NotStartedException();

	std::lock_guard<std::mutex>	lock(mtx);
	auto it = msgsToSend.find(clientID);
	if (it == msgsToSend.end())
		throw UnknownClientException();

	it->second.push(message);
}

void
Server::sendToArray(
	const Message& message,
	std::vector<ClientID> clientIDs
)
{
	if (!isStarted)
		throw NotStartedException();

	bool	error = false;

	for (ClientID clientID : clientIDs) {
		try {
			sendTo(message, clientID);
		} catch (const std::exception& e) {
			error = true;
		}
	}

	if (error)
		throw BatchSendingFailedException();
}

void
Server::sendToAll(
	const Message& message
)
{
	if (!isStarted)
		throw NotStartedException();

	bool	error = false;

	for (const auto& client : clients) {
		try {
			sendTo(message, client.first);
		} catch (const std::exception& e) {
			error = true;
		}
	}

	if (error)
		throw BatchSendingFailedException();
}

void
Server::update()
{
	if (!isStarted)
		throw NotStartedException();

	if (shouldEnd) {
		stop();
		throw NotStartedException();
	}

	std::vector<std::pair<ClientID, Message>>	processingList;
	std::unordered_map<Message::Type, Action>	actionsList;


	{
		std::lock_guard<std::mutex>	lock(mtx);
		processingList.swap(msgs);
		actionsList = actions;
	}

	for (auto& [clientID, msg] : processingList) {
		auto	it = actionsList.find(msg.type());
		if (it != actionsList.end())
			it->second(clientID, msg);
	}
}

void
Server::acceptConnection()
{
	sockaddr_in	clientAddr{};
	socklen_t	addrLen = sizeof(clientAddr);

	int	clientSock = accept(sockfd, (struct sockaddr*)&clientAddr, &addrLen);
	if (clientSock < 0)
		return ;

	ClientID	newClientID = static_cast<ClientID>(clientSock);
	Client		newClient;
	newClient.sockfd = clientSock;

	std::lock_guard<std::mutex> lock(mtx);
	clients[newClientID] = newClient;
	msgsToSend[newClientID] = std::queue<Message>();
}

std::map<Server::ClientID, Server::Client>::iterator
Server::receiveMsg(
	std::map<ClientID, Client>::iterator it
)
{
	Client& client = it->second;

	if (client.state == Client::NOSIZE) {
		client.bytesRead = recv(
			client.sockfd,
			&client.size + client.totalBytes,
			sizeof(client.size) - client.totalBytes,
			0
		);

		if (client.bytesRead <= 0) {
			close(client.sockfd);
			it = clients.erase(it);
			return it;
		}

		client.totalBytes += client.bytesRead;
		if (client.totalBytes < sizeof(client.size))
			return ++it;

		client.state = Client::SIZE;
		client.totalBytes = 0;
		client.data.resize(client.size, '\0');

	} else if (client.state == Client::SIZE) {
		client.bytesRead = recv(
			client.sockfd,
			client.data.data() + client.totalBytes,
			client.size - client.totalBytes,
			0
		);

		if (client.bytesRead <= 0) {
			close(client.sockfd);
			it = clients.erase(it);
			return it;
		}

		client.totalBytes += client.bytesRead;
		if (client.totalBytes < client.size)
			return ++it;

		client.state = Client::MESSAGE;
		client.totalBytes = 0;
	}

	if (client.state == Client::MESSAGE) {
		client.state = Client::NOSIZE;
		Message	msg(0);
		try {
			msg.deserialize(client.data);
		} catch (const std::exception& e) {
			return ++it;
		}

		auto t = msgs.emplace_back(it->first, std::move(msg));
	}

	return ++it;
}

void
Server::sendMsg(
	const Message& message,
	ClientID clientID
)
{
	if (!isStarted)
		throw NotStartedException();

	auto it = clients.find(clientID);
	if (it == clients.end())
		throw UnknownClientException();

	std::string	data = message.serialize();
	size_t		size = data.size();

	if (send(it->second.sockfd, &size, sizeof(size), 0) < 0)
		throw SendingFailedException();

	if (send(it->second.sockfd, data.c_str(), size, 0) < 0)
		throw SendingFailedException();
}

void
Server::receiveMsgs()
{
	fd_set	readfds;
	fd_set	writefds;
	while (isStarted) {
		FD_ZERO(&readfds);
		FD_ZERO(&writefds);
		FD_SET(sockfd, &readfds);
		int	max_fd = sockfd;

		{
			std::lock_guard<std::mutex>	lock(mtx);
			for (const auto& client : clients) {
				FD_SET(client.second.sockfd, &readfds);
				FD_SET(client.second.sockfd, &writefds);
				if (client.second.sockfd > max_fd)
					max_fd = client.second.sockfd;
			}
		}

		int	activity = select(max_fd + 1, &readfds, &writefds, nullptr, nullptr);
		if (activity < 0)
			continue ;

		if (FD_ISSET(sockfd, &readfds))
			acceptConnection();

		{
			std::lock_guard<std::mutex>	lock(mtx);
			for (auto it = clients.begin(); it != clients.end();) {
				if (FD_ISSET(it->second.sockfd, &readfds)) {
					it = receiveMsg(it);
					if (it == clients.end())
						break ;
				} else {
					it++;
				}
			}
		}

		{
			std::lock_guard<std::mutex>	lock(mtx);
			for (auto it = clients.begin(); it != clients.end(); it++) {
				if (FD_ISSET(it->second.sockfd, &writefds)) {
					auto	msgIt = msgsToSend.find(it->first);
					if (msgIt != msgsToSend.end() && !msgIt->second.empty()) {
						try {
							sendMsg(msgIt->second.front(), it->first);
						} catch (const std::exception&e) {}
						msgIt->second.pop();
					}
				}
			}
		}
	}
}
