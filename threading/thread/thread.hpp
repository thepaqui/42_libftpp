#ifndef LIBFTPP_THREAD_HPP
# define LIBFTPP_THREAD_HPP

# include <string>
# include <functional>
# include <thread>
# include <atomic>

class Thread {
private :
	std::thread				thread;
	std::string				threadName;
	std::function<void()>	func;
	// Atomic bool for thread-safety on r/w operations
	std::atomic<bool>		running;

public :
	Thread(const std::string& name, std::function<void()> functToExecute);

	void	start();
	void	stop();
};

#endif