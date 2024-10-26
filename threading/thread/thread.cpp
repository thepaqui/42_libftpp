#include "thread.hpp"
#include "../../iostream/iostream.hpp"

Thread::Thread(
	const std::string& name,
	Callback functToExecute
)
: threadName("[" + name + "] "), running(false)
{
	if (!functToExecute)
		throw NullFunctionException();

	func = std::move(functToExecute);
}

void
Thread::start()
{
	if (running || thread.joinable())
		return ;

	running = true;
	thread = std::thread([this]() {
		threadSafeCout.setPrefix(threadName);
		func();
	});
}

void
Thread::stop()
{
	if (!running || !thread.joinable())
		return ;

	running = false;
	thread.join();
}
