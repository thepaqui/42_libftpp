#ifndef LIBFTPP_PERSISTENT_WORKER_HPP
# define LIBFTPP_PERSISTENT_WORKER_HPP

# include <thread>
# include <mutex>
# include <condition_variable>
# include <atomic>
# include <unordered_map>
# include <string>
# include <functional>
# include <chrono>

using Callback = std::function<void()>;

class PersistentWorker {
private :
	std::atomic<bool>							running;
	std::thread									thread;
	std::mutex									mtx;
	std::condition_variable						cv;
	std::unordered_map<std::string, Callback>	tasks;

	void	loop();

public :
	PersistentWorker();
	~PersistentWorker();

	void	addTask(const std::string& name, const Callback& jobToExecute);
	void	removeTask(const std::string& name);
};

#endif