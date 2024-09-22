#include "pool.hpp"
#include <iostream>

class TestObject {
public:
	TestObject() { std::cout << "TestObject default constructor" << std::endl; }
	TestObject(int value)
	{
		std::cout << "TestObject constructor with value [" << value << "]" << std::endl;
		_val1 = value;
	}
	TestObject(int val1, int val2)
	{
		std::cout << "TestObject constructor with values [" << val1 << ", " << val2 << "]" << std::endl;
		_val1 = val1;
		_val2 = val2;
	}
	~TestObject() { std::cout << "TestObject destructor" << std::endl; }

	int _val1 = 0;
	int _val2 = 0;

	void sayHello() const { std::cout << "Hello from TestObject" << std::endl; }
	void printInfo() const { std::cout << "TestObject Info [" << _val1 << ", " << _val2 << "]" << std::endl; }
};

int main() {
	// Create a Pool for TestObject
	Pool<TestObject> myPool;

	// Resize the pool to pre-allocate 5 objects
	// Should output the 5 "TestObject default constructor"
	myPool.resize(5);

	// Acquire an object from the pool
	// Should output "TestObject constructor with value [15]"
	Pool<TestObject>::Object obj1 = myPool.acquire(15);
	obj1->sayHello(); // Should output: "Hello from TestObject"
	obj1->printInfo(); // Should output: "TestObject Info [15, 0]"

	{
		// Acquire another object in a different scope
		// Should output "TestObject default constructor"
		Pool<TestObject>::Object obj2 = myPool.acquire();
		obj2->sayHello(); // Should also output: "Hello from TestObject"
		obj2->printInfo(); // Should output: "TestObject Info [0, 0]"
		// obj2 is released back to the pool when it goes out of scope
	}

	// Acquire another object; this should give us the object that obj2 pointed to
	// Should output "TestObject default constructor"
	Pool<TestObject>::Object obj3 = myPool.acquire(6, 9);
	obj3->sayHello(); // Should output: "Hello from TestObject"
	obj3->printInfo(); // Should output: "TestObject Info [6, 9]"

	std::cout << "Resizing pool to 2" << std::endl;
	try
	{
		myPool.resize(2);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		std::cout << "Could not resize pool to 2" << std::endl;
		return 1;
	}
	std::cout << "Resized pool to 2" << std::endl;

/*
	std::cout << "Resizing pool to 1" << std::endl;
	try
	{
		myPool.resize(1);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		std::cout << "Could not resize pool to 1" << std::endl;
		return 1;
	}
	std::cout << "Resized pool to 1" << std::endl;
*/

	try
	{
		std::cout << "Acquiring an object that should fail" << std::endl;
		Pool<TestObject>::Object obj4 = myPool.acquire(69, 420);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		std::cout << "Could not acquire the object" << std::endl;
	}

	// obj1 and obj3 are released back to the pool when the program ends
	// Should output the 2 "TestObject default destructor"
	return 0;
}

