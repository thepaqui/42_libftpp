#include "chronometer.hpp"

// Constructs with given precision `mode`
Chronometer::Chronometer(
	Precision mode
)
: mode(mode), state(State::SET)
{}

// Starts chronometer
// Resets chronometer if it was used before without being reset
// It resets without changing the precision
// Throws if chronometer was already started
void
Chronometer::start()
{
	if (state == State::STARTED)
		throw StartTwiceException();

	if (state == State::STOPPED)
		reset();

	startTime = std::chrono::steady_clock::now();
	state = State::STARTED;
}

// Stops chronometer
// Throws if chronometer was already stopped without being reset
// Throws if chronometer was not started
void
Chronometer::stop()
{
	if (state == State::STOPPED)
		throw StopTwiceException();

	if (state == State::SET)
		throw NoStartException();

	stopTime = std::chrono::steady_clock::now();
	state = State::STOPPED;
}

// Resets chronometer
// Does not change precision
void
Chronometer::reset() noexcept
{
	duration = -1.0;
	state = State::SET;
}

// Resets chronometer
// Changes precision to `newMode`
void
Chronometer::reset(
	Precision newMode
) noexcept
{
	duration = -1.0;
	mode = newMode;
	state = State::SET;
}

// Changes precision to `newMode`
void
Chronometer::setPrecision(
	Precision newMode
) noexcept
{
	mode = newMode;
}

// Returns measured time in chronometer's precision
// Throws if chronometer was unused
// Throws if chronometer was started but not stopped
double
Chronometer::getDuration()
{
	if (state == State::SET)
		throw NoStartException();

	if (state == State::STARTED)
		throw NoStopException();

	calculateDuration();
	return duration;
}

// Changes precision to `convertTo`, even if it throws
// Returns measured time in chronometer's new precision
// Throws if chronometer was unused
// Throws if chronometer was started but not stopped
double
Chronometer::getDuration(
	Precision convertTo
)
{
	mode = convertTo;
	return getDuration();
}

double
Chronometer::getDuration_nsec() const
{
	auto duration_nsec = std::chrono::duration<double, std::nano>(
		stopTime - startTime
	).count();
	return duration_nsec;
}

void
Chronometer::calculateDuration()
{
	duration = getDuration_nsec();
	switch (mode)
	{
		case Precision::USEC:
			duration /= THOUSAND;
			break;
		case Precision::MSEC:
			duration /= MILLION;
			break;
		case Precision::SEC:
			duration /= BILLION;
			break;
		default :
			break;
	}
}
