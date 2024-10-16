#ifndef LIBFTPP_TIMER_HPP
# define LIBFTPP_TIMER_HPP

# include <exception>
# include <ctime>
# include <sys/time.h>
# include <chrono>

class Chronometer
{
public :
	enum class Precision
	{
		SEC = 0,// Seconds
		MSEC = 1,// Millieconds
		USEC = 2,// Microseconds
		NSEC = 3// Nanoseconds
	};

private	:
	enum class State
	{
		SET,
		STARTED,
		STOPPED
	};

	static constexpr double LF_THOUSAND	= 1000.0;
	static constexpr double LF_MILLION	= 1000000.0;
	static constexpr double LF_BILLION	= 1000000000.0;

	using Timestamp = std::chrono::steady_clock::time_point;

	Precision	mode;
	State		state;
	Timestamp	startTime;
	Timestamp	stopTime;
	double		duration = -1.0;

	void	calculateDuration();
	double	getDuration_nsec() const;

public	:
	// Constructs with precision of seconds
	Chronometer();
	// Constructs with given precision
	Chronometer(Precision mode);
	// Default destructor
	~Chronometer() = default;

	/* Methods */

	// Starts chronometer
	// Resets chronometer if it was used before without being reset
	// It resets without changing the precision
	// Throws if chronometer was already started
	// Throws if clock_gettime() failed
	void	start();

	// Stops chronometer
	// Throws if chronometer was not started
	// Throws if chronometer was already stopped without being reset
	// Throws if clock_gettime() failed
	void	stop();

	// Resets chronometer
	// Does not change precision
	void	reset() noexcept;

	// Resets chronometer
	// Changes precision to given one
	void	reset(Precision newMode) noexcept;

	// Changes precision to given one
	void	setPrecision(Precision mode) noexcept;

	// Returns measured time in chronometer's precision
	// Throws if chronometer was just reset
	// Throws if chronometer was started but not stopped
	[[nodiscard]] double	getDuration();

	// Changes precision to given one, even if it throws
	// Returns measured time in chronometer's new precision
	// Throws if chronometer was just reset
	// Throws if chronometer was started but not stopped
	[[nodiscard]] double	getDuration(Precision convertTo);

	/* Exceptions */

	class StartTwiceException : public std::exception {
	public:
		const char *what() const noexcept { return "Chronometer: Already started."; }
	};

	class StopTwiceException : public std::exception {
	public:
		const char *what() const noexcept { return "Chronometer: Already stopped."; }
	};

	class NoStartException : public std::exception {
	public:
		const char *what() const noexcept { return "Chronometer: Not started."; }
	};

	class NoStopException : public std::exception {
	public:
		const char *what() const noexcept { return "Chronometer: Not stopped."; }
	};
};

#endif