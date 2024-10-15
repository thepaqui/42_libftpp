#include "client.hpp"
#include "../../iostream/iostream.hpp"
#include <string>

int main() {
	Client client;

	client.defineAction(3, [](Message& msg){
		int doubledValue;
		msg >> doubledValue;
		threadSafeCout << "Received a doubled value: " << doubledValue << std::endl;
	});

	try {
		// Connect to the server
		client.connect("localhost", 8080);
	} catch (const std::exception &e) {
		threadSafeCout << e.what() << std::endl;
	}

	// Send a message of type 1 (int)
	Message message1(1);
	message1 << 42;
	try {
		client.send(message1);
	} catch (const std::exception& e) {
		threadSafeCout << e.what() << std::endl;
	}

	// Send a message of type 2 (size_t followed by characters)
	Message message2(2);
	std::string str = "Hello";
	message2 << str.size();
	for (char c : str) {
		message2 << c;
	}
	try {
		client.send(message2);
	} catch (const std::exception& e) {
		threadSafeCout << e.what() << std::endl;
	}

	std::string	input = "";
	while (input.compare("quit") && input.compare("q"))
	{
		if (!input.empty()) {
			Message	msg(2);
			msg << input.size();
			for (char c : input) {
				msg << c;
			}
			try {
				client.send(msg);
			} catch (const std::exception& e) {
				threadSafeCout << e.what() << std::endl;
			}
		}

		try {
			client.update();
		} catch (const std::exception& e) {
			threadSafeCout << e.what() << std::endl;
			break ;
		}

		threadSafeCout << "Client updated." << std::endl;
		threadSafeCout << "Available operations :" << std::endl;
		threadSafeCout << " - [Q]uit : close the program" << std::endl;
		threadSafeCout << " - Any other input to send message" << std::endl;

		input = "";
		std::getline(std::cin, input);

		std::transform(input.begin(), input.end(), input.begin(), 
			[](unsigned char c){ return std::tolower(c); });
	}

	// Disconnect from the server
	client.disconnect();
	threadSafeCout << "Client disconnected." << std::endl;

	return 0;
}
