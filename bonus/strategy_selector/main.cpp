#include "strategy_selector.hpp"
#include <iostream>

int main()
{
	int	val1 = 7;

	StrategySelector<int> strat([](int& val) {
		std::cout << "Value: " << val << std::endl;
	});
	// Should print: "Value: 7"
	strat.handle(val1);

	strat.setDefaultStrategy(nullptr);
	// Should print nothing
	strat.handle(val1);

	strat.setDefaultStrategy([](int& val) {
		std::cout << "Value of int: " << val << std::endl;
	});
	// Should print: "Value of int: 7"
	strat.handle(val1);

	strat.addStrategy(
		[](int& val) {
			std::cout << "THIS SHOULD NEVER PRINT! (" << val << ")" << std::endl;
		},
		nullptr
	);

	strat.addStrategy(
		[](int& val) {
			std::cout << "Value of int (negative): " << val << std::endl;
		},
		[](const int& val) -> bool {
			return (val < 0);
		}
	);

	strat.addStrategy(
		[](int& val) {
			std::cout << "Value of int (> 5000): " << val << std::endl;
		},
		[](const int& val) -> bool {
			return (val > 5000);
		}
	);

	strat.addStrategy(
		[](int& val) {
			std::cout << val << " GANG!" << std::endl;
		},
		[](const int& val) -> bool {
			return (val == 42);
		}
	);

	strat.addStrategy(
		[](int& val) {
			std::cout << val << " GANG!" << std::endl;
		},
		[](const int& val) -> bool {
			return (val == 424242);
		}
	);

	int	val2 = -1;
	int	val3 = 5138008;
	int	val4 = 42;
	int	val5 = 424242;

	// Should print: "Value of int (negative): -1"
	strat.handle(val2);
	// Should print: "Value of int (> 5000): 5138008"
	strat.handle(val3);
	// Should print: "42 GANG!"
	strat.handle(val4);
	// Should print: "Value of int (> 5000): 424242"
	strat.handle(val5);

	strat.setMode(StrategySelector<int>::Mode::ALL);

	// Should print: "Value of int (> 5000): 424242"
	// Should print: "424242 GANG!"
	// Should print: "Value of int: 424242"
	strat.handle(val5);

	strat.setMode(StrategySelector<int>::Mode::ALL_NO_DEFAULT);

	// Should print: "Value of int (> 5000): 424242"
	// Should print: "424242 GANG!"
	strat.handle(val5);

	// This should throw
	try {
		strat.setMode(static_cast<StrategySelector<int>::Mode>(42));
	} catch (const std::exception& e) {
		std::cout << "Exception caught: " << e.what() << std::endl;
	}

	// Should print nothing
	strat.handle(val1);

	strat.setMode(StrategySelector<int>::Mode::FIRST);

	// Should print "Value of int: 7"
	strat.handle(val1);

	return 0;
}
