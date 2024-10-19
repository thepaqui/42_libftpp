#ifndef LIBFTPP_SERVER_HPP
# define LIBFTPP_SERVER_HPP

# include "../message/message.hpp"
# include <functional>
# include <vector>
# include <queue>
# include <atomic>
# include <thread>
# include <mutex>
# include <map>
# include <unordered_map>
# include <stdexcept>
# include <unistd.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <fcntl.h>
# include <sys/select.h>

class Server {
public :
	using ClientID = long long;
	using Action = std::function<void(ClientID& clientID, Message& msg)>;

	Server();
	~Server();

	void	start(const size_t& p_port);

	void	defineAction(const Message::Type& messageType, const Action& action);
	void	sendTo(const Message& message, ClientID clientID);
	void	sendToArray(const Message& message, std::vector<ClientID> clientIDs);
	void	sendToAll(const Message& message);

	void	update();

private :
	struct Client {
		enum State {
			NOSIZE,
			SIZE,
			MESSAGE
		};
		int			sockfd;
		State		state{State::NOSIZE};
		size_t		size;
		std::string	data;
		ssize_t		bytesRead;
		size_t		totalBytes = 0;
	};

	int							sockfd;
	std::map<ClientID, Client>	clients;
	std::atomic<bool>			isStarted;
	std::atomic<bool>			shouldEnd{false};

	std::thread									receiver;
	std::mutex									mtx;
	std::vector<std::pair<ClientID, Message>>	msgs;
	std::map<ClientID, std::queue<Message>>		msgsToSend;

	std::unordered_map<Message::Type, Action>	actions;

	void	stop();

	void									receiveMsgs();
	void									acceptConnection();
	std::map<ClientID, Client>::iterator	receiveMsg(std::map<ClientID, Client>::iterator it);
	void									sendMsg(const Message& message, ClientID clientID);

public :
	/* Exceptions */
	class AlreadyStartedException : public std::exception {
		const char*	what() const noexcept { return "Server: Already started."; };
	};

	class NotStartedException : public std::exception {
		const char*	what() const noexcept { return "Server: Not started."; };
	};

	class StartFailedException : public std::runtime_error {
	public :
		explicit StartFailedException(const std::string& msg)
		: runtime_error("Server: " + msg + ".") {}
	};

	class UnknownClientException : public std::runtime_error {
	public :
		explicit UnknownClientException()
		: runtime_error("Server: Unknown client.") {}
	};

	class SendingFailedException : public std::runtime_error {
	public :
		explicit SendingFailedException()
		: runtime_error("Server: Failed to send message.") {}
	};

	class BatchSendingFailedException : public std::runtime_error {
	public :
		explicit BatchSendingFailedException()
		: runtime_error("Server: Failed to send at least 1 message.") {}
	};
};

#endif