#include "chronometer.hpp"

Chronometer::Chronometer()
: mode(Precision::SEC), state(State::SET)
{}

Chronometer::Chronometer(
	Precision mode
)
: mode(mode), state(State::SET)
{}

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

void
Chronometer::reset() noexcept
{
	duration = -1.0;
	state = State::SET;
}

void
Chronometer::reset(
	Precision newMode
) noexcept
{
	duration = -1.0;
	mode = newMode;
	state = State::SET;
}

void
Chronometer::setPrecision(
	Precision newMode
) noexcept
{
	mode = newMode;
}

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
			duration /= LF_THOUSAND;
			break;
		case Precision::MSEC:
			duration /= LF_MILLION;
			break;
		case Precision::SEC:
			duration /= LF_BILLION;
			break;
		default :
			break;
	}
}
