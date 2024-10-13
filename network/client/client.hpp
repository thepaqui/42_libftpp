#ifndef LIBFTPP_CLIENT_HPP
# define LIBFTPP_CLIENT_HPP

# include "../message/message.hpp"
# include <string>
# include <unordered_map>
# include <functional>
# include <memory>
# include <vector>
# include <atomic>
# include <thread>
# include <mutex>
# include <stdexcept>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>

class Client {
public :
	using	Action = std::function<void(Message& msg)>;

	Client();
	~Client();

	void	connect(const std::string& address, const size_t& port);
	void	disconnect();

	void	send(const Message& message);
	void	defineAction(const Message::Type& messageType, const Action& action);

	void	update();

	/* Exceptions */

private :
	int					sockfd;
	std::atomic<bool>	isConnected;
	std::atomic<bool>	shouldEnd{false};

	std::thread				receiver;
	std::mutex				mtx;
	std::vector<Message>	msgs;

	std::unordered_map<Message::Type, Action>	actions;

	void	receiveMsgs();

public :
	/* Exceptions */
	class AlreadyConnectedException : public std::exception {
		const char*	what() const noexcept { return "Client: Already connected."; };
	};

	class NotConnectedException : public std::exception {
		const char*	what() const noexcept { return "Client: Not connected."; };
	};

	class ConnectionFailedException : public std::runtime_error {
	public :
		explicit ConnectionFailedException(const std::string& msg)
		: runtime_error("Client: " + msg + ".") {}
	};

	class SendingFailedException : public std::runtime_error {
	public :
		explicit SendingFailedException()
		: runtime_error("Client: Failed to send message.") {}
	};
};

#endif