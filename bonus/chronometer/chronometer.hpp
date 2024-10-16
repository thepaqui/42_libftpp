#ifndef LIBFTPP_CHRONOMETER_HPP
# define LIBFTPP_CHRONOMETER_HPP

# include <exception>
# include <chrono>

class Chronometer
{
public :
	enum class Precision
	{
		SEC = 0,	// Seconds
		MSEC = 1,	// Millieconds
		USEC = 2,	// Microseconds
		NSEC = 3	// Nanoseconds
	};

private	:
	enum class State
	{
		SET,
		STARTED,
		STOPPED
	};

	static constexpr double THOUSAND	= 1000.0;
	static constexpr double MILLION		= 1000000.0;
	static constexpr double BILLION		= 1000000000.0;

	using Timestamp = std::chrono::steady_clock::time_point;

	Precision	mode;
	State		state;
	Timestamp	startTime;
	Timestamp	stopTime;
	double		duration = -1.0;

	void	calculateDuration();
	double	getDuration_nsec() const;

public	:
	Chronometer(Precision mode = Precision::SEC);
	~Chronometer() = default;

	void	start();
	void	stop();

	void	reset() noexcept;
	void	reset(Precision newMode) noexcept;

	void	setPrecision(Precision mode) noexcept;

	[[nodiscard]] double	getDuration();
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