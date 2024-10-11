#ifndef LIBFTPP_THREAD_SAFE_IOSTREAM_TPP
# define LIBFTPP_THREAD_SAFE_IOSTREAM_TPP

# include "thread_safe_iostream.hpp"

template <typename TType>
ThreadSafeIOStream&
ThreadSafeIOStream::operator<<(
	const TType& data
)
{
	outputBuffer << data;

	if (outputUnitBuf)
		flush();

	return *this;
}

template <typename TType>
ThreadSafeIOStream&
ThreadSafeIOStream::operator>>(
	TType& data
)
{
	std::string	line;
	{
		std::lock_guard<std::mutex>	lock(mtx);
		std::getline(std::cin, line);
	}
	inputBuffer.str(line);
	inputBuffer.clear();
	inputBuffer >> data;
	return *this;
}

template <typename TType>
ThreadSafeIOStream&
ThreadSafeIOStream::operator>>(
	TType&& manip
)
{
	inputBuffer >> std::forward<TType>(manip);
	return *this;
}

#endif