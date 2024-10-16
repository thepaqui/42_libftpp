#ifndef LIBFTPP_TIMER_HPP
# define LIBFTPP_TIMER_HPP

# include <exception>
# include <chrono>

class Timer {
public :
	enum class Precision
	{
		SEC = 0,	// Seconds
		MSEC = 1,	// Millieconds
		USEC = 2,	// Microseconds
		NSEC = 3	// Nanoseconds
	};

private :
	enum class State
	{
		SET,
		STARTED
	};

	static constexpr unsigned long long THOUSAND	= 1000;
	static constexpr unsigned long long MILLION		= 1000000;
	static constexpr unsigned long long BILLION		= 1000000000;

	using Timestamp = std::chrono::steady_clock::time_point;

	State				state;
	Timestamp			startTime;
	Timestamp			stopTime;
	unsigned long long	duration; // nanoseconds

public :
	Timer(unsigned int duration, Precision mode = Precision::SEC);
	~Timer() = default;

	void	start();
	bool	isOver() const;

	void	reset(unsigned int duration, Precision newMode = Precision::SEC) noexcept;

	/* Exceptions */

	class StartTwiceException : public std::exception {
	public:
		const char *what() const noexcept { return "Timer: Already started."; }
	};

	class NoStartException : public std::exception {
	public:
		const char *what() const noexcept { return "Timer: Not started."; }
	};
};

#endif