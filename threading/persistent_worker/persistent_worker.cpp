#include "persistent_worker.hpp"

PersistentWorker::PersistentWorker()
: running(true), thread(std::thread(&PersistentWorker::loop, this))
{}

PersistentWorker::~PersistentWorker()
{
	running = false;
	cv.notify_all();
	if (thread.joinable())
		thread.join();
}

void
PersistentWorker::loop()
{
	while (running) {
		Callback	task;
		{
			std::unique_lock<std::mutex>	lock(mtx);
			cv.wait(lock, [this] { return !tasks.empty() || !running; });

			if (!running)
				return ;

			for (const auto& [name, func] : tasks) {
				task = func;
				if (task)
					task();
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void
PersistentWorker::addTask(
	const std::string& name,
	const Callback& jobToExecute
)
{
	std::lock_guard<std::mutex>	lock(mtx);
	tasks[name] = jobToExecute;
	cv.notify_one();
}

void
PersistentWorker::removeTask(
	const std::string& name
)
{
	std::lock_guard<std::mutex>	lock(mtx);
	tasks.erase(name);
}
