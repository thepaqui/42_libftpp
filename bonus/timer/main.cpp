#include "timer.hpp"
#include "../chronometer/chronometer.hpp"
#include <iostream>
#include <iomanip>

#define WAIT_TIME 5
constexpr Timer::Precision			precision = Timer::Precision::SEC;
constexpr Chronometer::Precision	precisionChrono = Chronometer::Precision::SEC;

int main()
{
	std::string	unit;
	int	displayPrecision = 0;
	switch (precision) {
		case Timer::Precision::SEC:
			unit = "seconds";
			displayPrecision = 9;
			break ;
		case Timer::Precision::MSEC:
			unit = "milliseconds";
			displayPrecision = 6;
			break ;
		case Timer::Precision::USEC:
			unit = "microseconds";
			displayPrecision = 3;
			break ;
		default :
			unit = "nanoseconds";
			break ;
	}
	std::cout << "Waiting for " << WAIT_TIME << " " << unit << "." << std::endl;

	Timer	timer(WAIT_TIME, precision);

	Chronometer	chronometer(precisionChrono);
	chronometer.start();

	timer.start();
	while (!timer.isOver());

	chronometer.stop();
	double duration = chronometer.getDuration();

	std::cout << "Waiting's over!" << std::endl;
	std::cout << "Actually took " << std::fixed << std::setprecision(displayPrecision) << duration << " " << unit << std::endl;

	return 0;
}
