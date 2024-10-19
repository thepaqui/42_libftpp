#include <iostream>
#include "singleton.hpp"

class MyClass : public Singleton<MyClass> {
private:
	friend class Singleton<MyClass>;
	MyClass(int value)
	{
		std::cout << "MyClass constructor, with value [" << value << "]" << std::endl;
	}
public:
	void printMessage() {
		std::cout << "Hello from MyClass" << std::endl;
	}
};

int main() {
//		MyClass	test(69); // This won't compile
//		Singleton<MyClass>	test2; // This won't compile either

		try
		{
				// This should throw an exception as instance is not yet created
				MyClass::instance();
		} catch (const std::exception& e) {
				std::cout << e.what() << std::endl; // Output: "Singleton: Instance not yet created"
		}

		MyClass::instantiate(42); // Setting up the instance

		MyClass::instance()->printMessage(); // Output: "Hello from MyClass"

		try
		{
				// This should throw an exception as instance is already created
				MyClass::instantiate(100);
		} catch (const std::exception& e) {
				std::cout << e.what() << std::endl; // Output: "Singleton: Instance already created"
		}

		return 0;
}
