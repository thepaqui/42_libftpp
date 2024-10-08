#ifndef LIBFTPP_THREAD_SAFE_IOSTREAM_TPP
# define LIBFTPP_THREAD_SAFE_IOSTREAM_TPP

# include "thread_safe_iostream.hpp"

template <typename TType>
ThreadSafeIOStream&
ThreadSafeIOStream::operator<<(
	const TType& data
)
{
	// A lock_guard's destructor releases the mutex
	// So outputMutex will be unlocked when lock goes out of scope
	std::lock_guard<std::mutex>	lock(outputMutex);

	// Using ostringstream to concatenate prefix
	// and data before sending to cout
	std::ostringstream	oss;
	oss << outputPrefix << data;
	std::cout << oss.str();

	return *this;
}

template <typename TType>
ThreadSafeIOStream&
ThreadSafeIOStream::operator>>(
	TType& data
)
{
	std::lock_guard<std::mutex>	lock(outputMutex);

	std::cin >> data;

	return *this;
}

#endif