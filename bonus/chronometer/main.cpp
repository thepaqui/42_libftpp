#include "chronometer.hpp"
#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
#include <cstdlib>
#include <ctime>

#define CONTAINER_SIZE 10000000
constexpr Chronometer::Precision	precision = Chronometer::Precision::MSEC;

int main()
{
	if (CONTAINER_SIZE <= 0)
		return -1;

	std::vector<int>	vec;
	std::list<int>		lst;
	try {
		vec.reserve(CONTAINER_SIZE);

		for (int i = 0; i < CONTAINER_SIZE; i++) {
			vec.push_back(i);
			lst.push_back(i);
		}
	} catch (const std::exception& e) {
		std::cout << "CONTAINER_SIZE is too big." << std::endl;
		return -1;
	}

	srand(time(0));

	int	toFind = rand() % CONTAINER_SIZE;
	std::cout << "Int to find: " << toFind << '\n' << std::endl;

	std::cout << std::fixed;
	std::cout << "Precision: ";
	std::string	symbol;
	int	displayPrecision = 0;
	switch (precision) {
		case Chronometer::Precision::SEC:
			symbol = "s";
			displayPrecision = 9;
			std::cout << "Seconds" << std::endl;
			break ;
		case Chronometer::Precision::MSEC:
			symbol = "ms";
			displayPrecision = 6;
			std::cout << "Milliseconds" << std::endl;
			break ;
		case Chronometer::Precision::USEC:
			symbol = "us";
			displayPrecision = 3;
			std::cout << "Microseconds" << std::endl;
			break ;
		default :
			symbol = "ns";
			std::cout << "Nanoseconds" << std::endl;
			break ;
	}

	Chronometer	chronometer(precision);

	try {
		chronometer.start();
	} catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
		return -1;
	}

	for (const int& i : vec)
		if (i == toFind)
			break ;

	try {
		chronometer.stop();
	} catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
		return -1;
	}

	const double vecTime = chronometer.getDuration();
	std::cout << "Time for vector: " << std::setprecision(displayPrecision) << vecTime << " " << symbol << std::endl;

	chronometer.reset();

	try {
		chronometer.start();
	} catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
		return -1;
	}

	for (const int& i : lst)
		if (i == toFind)
			break ;

	try {
		chronometer.stop();
	} catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
		return -1;
	}

	const double lstTime = chronometer.getDuration();
	std::cout << "Time for list:   " << std::setprecision(displayPrecision) << lstTime << " " << symbol << std::endl;

	return 0;
}
