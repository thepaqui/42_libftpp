#include "timer.hpp"

// Constructs with given `duration` expressed in precision `mode`
Timer::Timer(
	unsigned int duration,
	Precision mode
)
{
	reset(duration, mode);
}

// Starts timer
// Throws if the timer was already started
void
Timer::start()
{
	if (state == State::STARTED)
		throw StartTwiceException();

	startTime = std::chrono::steady_clock::now();
	stopTime = startTime + std::chrono::nanoseconds(duration);
	state = State::STARTED;
}

// Returns true if the timer ended, false otherwise
// Throws if the timer was not started
bool
Timer::isOver() const
{
	if (state == State::SET)
		throw NoStartException();

	return (std::chrono::steady_clock::now() >= stopTime);
}

// Constructs with given `newDuration` expressed in precision `newMode`
void
Timer::reset(
	unsigned int newDuration,
	Precision newMode
) noexcept
{
	state = State::SET;
	switch (newMode) {
		case Precision::SEC:
			duration = static_cast<unsigned long long>(newDuration) * BILLION;
			break ;
		case Precision::MSEC:
			duration = static_cast<unsigned long long>(newDuration) * MILLION;
			break ;
		case Precision::USEC:
			duration = static_cast<unsigned long long>(newDuration) * THOUSAND;
			break ;
		default:
			duration = static_cast<unsigned long long>(newDuration);
			break ;
	}
}
