#ifndef LIBFTPP_THREAD_HPP
# define LIBFTPP_THREAD_HPP

# include <string>
# include <functional>
# include <thread>
# include <atomic>

using Callback = std::function<void()>;

class Thread {
private :
	std::thread				thread;
	std::string				threadName;
	Callback				func;
	// Atomic bool for thread-safety on r/w operations
	std::atomic<bool>		running;

public :
	Thread(const std::string& name, Callback functToExecute);

	void	start();
	void	stop();
};

#endif