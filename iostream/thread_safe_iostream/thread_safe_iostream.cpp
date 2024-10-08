#include "thread_safe_iostream.hpp"

std::mutex					ThreadSafeIOStream::outputMutex;
thread_local std::string	ThreadSafeIOStream::outputPrefix;
thread_local std::string	ThreadSafeIOStream::outputBuffer;

thread_local ThreadSafeIOStream	threadSafeCout;

void
ThreadSafeIOStream::setPrefix(
	const std::string& prefix
)
{
	outputPrefix = prefix;
}

ThreadSafeIOStream&
ThreadSafeIOStream::operator<<(
	std::ostream& (*manip)(std::ostream&)
)
{
	std::lock_guard<std::mutex>	lock(outputMutex);

	// No need to bufferize a manipulator before sending it
	// since it's not actually data and does not need to get prefixed
	std::cout << manip;

	return *this;
}
