#include "thread_safe_iostream.hpp"

std::mutex						ThreadSafeIOStream::outputMutex;
thread_local std::string		ThreadSafeIOStream::outputPrefix;
thread_local std::ostringstream	ThreadSafeIOStream::outputBuffer;
thread_local bool				ThreadSafeIOStream::outputUnitBuf = false;

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
	std::ios_base& (*manip)(std::ios_base&)
)
{
	manip(outputBuffer);

	if (manip == static_cast<std::ios_base& (*)(std::ios_base&)>(std::unitbuf))
		outputUnitBuf = true;
	else if (manip == static_cast<std::ios_base& (*)(std::ios_base&)>(std::nounitbuf))
		outputUnitBuf = false;

	return *this;
}

ThreadSafeIOStream&
ThreadSafeIOStream::operator<<(
	std::ostream& (*manip)(std::ostream&)
)
{
	manip(outputBuffer);

	// TODO: Check if std::flush_emit is implemented on eval machine
	// and handle it if yes, as it is a flushing manip
	if (outputUnitBuf
		|| manip == static_cast<std::ostream& (*)(std::ostream&)>(std::endl)
		|| manip == static_cast<std::ostream& (*)(std::ostream&)>(std::flush)
//		|| manip == static_cast<std::ostream& (*)(std::ostream&)>(std::flush_emit)
	)
	{
		flush();
	}

	return *this;
}

ThreadSafeIOStream&
ThreadSafeIOStream::operator>>(
	std::istream& (*manip)(std::istream&)
)
{
	std::lock_guard<std::mutex>	lock(outputMutex);
	manip(std::cin);
	return *this;
}

void
ThreadSafeIOStream::flush()
{
	const std::string	output = outputPrefix + outputBuffer.str();
	outputBuffer.str("");
	outputBuffer.clear();

	// A lock_guard's destructor releases the mutex
	// So outputMutex will be unlocked when lock goes out of scope
	std::lock_guard<std::mutex>	lock(outputMutex);
	std::cout << output << std::flush;
}
