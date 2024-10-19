#include "observable_value.hpp"
#include <iostream>

int main() {
	ObservableValue<int> observableValue(10);

	auto subscription1 = observableValue.subscribe([&]() {
		std::cout << "Subscription 1: Value changed to " << observableValue.getValue() << std::endl;
	});

	auto subscription2 = observableValue.subscribe([&]() {
		std::cout << "Subscription 2: Doubled value now is " << observableValue.getValue() * 2 << std::endl;
	});

	observableValue = 20;

	observableValue.unsubscribe(subscription2);

	observableValue = 30;

	auto subscription3 = observableValue.subscribe([&]() {
		std::cout << "Subscription 3: Tripled value now is " << observableValue.getValue() * 3 << std::endl;
	});

	observableValue.setValue(100);

	std::cout << "\nFinal value: " << observableValue.getValue() << '\n' << std::endl;

	observableValue.unsubscribe(subscription1);
	observableValue.unsubscribe(subscription3);

	try {
		observableValue.unsubscribe(42);
	} catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	return 0;
}
