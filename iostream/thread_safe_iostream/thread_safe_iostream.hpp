#ifndef LIBFTPP_THREAD_SAFE_IOSTREAM_HPP
# define LIBFTPP_THREAD_SAFE_IOSTREAM_HPP

# include <iostream>
# include <sstream>
# include <string>
# include <mutex>

class ThreadSafeIOStream {
private :
	static std::mutex						mtx;
	thread_local static std::string			outputPrefix;
	thread_local static std::ostringstream	outputBuffer;
	thread_local static bool				outputUnitBuf;
	thread_local static std::istringstream	inputBuffer;

	void	flush();

public :
	void	setPrefix(const std::string& prefix);

	template <typename TType>
	ThreadSafeIOStream& operator<<(const TType& data);
	// For manipulators like unitbuf or hex
	ThreadSafeIOStream& operator<<(std::ios_base& (*manip)(std::ios_base&));
	// For manipulators like endl or flush
	ThreadSafeIOStream& operator<<(std::ostream& (*manip)(std::ostream&));

	template <typename TType>
	ThreadSafeIOStream& operator>>(TType& data);
	// For manipulators like unitbuf or hex
	ThreadSafeIOStream& operator>>(std::ios_base& (*manip)(std::ios_base&));
	// For ws manipulator
	ThreadSafeIOStream& operator>>(std::istream& (*manip)(std::istream&));
	// For other manipulators
	template <typename TType>
	ThreadSafeIOStream& operator>>(TType&& manip);
};

thread_local extern ThreadSafeIOStream	threadSafeCout;

# include "thread_safe_iostream.tpp"

#endif