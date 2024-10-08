#ifndef LIBFTPP_THREAD_SAFE_IOSTREAM_HPP
# define LIBFTPP_THREAD_SAFE_IOSTREAM_HPP

# include <iostream>
# include <sstream>
# include <string>
# include <mutex>

class ThreadSafeIOStream {
private :
	static std::mutex				outputMutex;
	thread_local static std::string	outputPrefix;
	thread_local static std::string	outputBuffer;

public :
	void	setPrefix(const std::string& prefix);

	template <typename TType>
	ThreadSafeIOStream& operator<<(const TType& data);

	// For manipulators like endl or flush
	ThreadSafeIOStream& operator<<(std::ostream& (*manip)(std::ostream&));

	template <typename TType>
	ThreadSafeIOStream& operator>>(TType& data);
};

thread_local extern ThreadSafeIOStream	threadSafeCout;

# include "thread_safe_iostream.tpp"

#endif