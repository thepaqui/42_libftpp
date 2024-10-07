#include "memento.hpp"
#include <iostream>

class TestClass : public Memento {
    friend class Memento;

public:
    int x;
    std::string y;

private:
    void _saveToSnapshot(Snapshot& snapshot) override {
        snapshot << x << y;
    }

    void _loadFromSnapshot(Snapshot& snapshot) override {
        snapshot >> x >> y;
    }

};

int main() {
    TestClass myObject;
    myObject.x = 42;
    myObject.y = "Hello";

    // Save the current state
    TestClass::Snapshot savedState = myObject.save();

    // Modify the object
    myObject.x = 100;
    myObject.y = "World";

    // Save the current state
    TestClass::Snapshot savedState2 = myObject.save();

    // Output the modified object
    // Expected Output: "Current state: x = 100, y = World"
    std::cout << "Current state: x = " << myObject.x << ", y = " << myObject.y << std::endl;

    // Restore the object to its saved state
    myObject.load(savedState);
    
    // Output the restored object
	// Expected Output: "Restored state: x = 42, y = Hello"
    std::cout << "Restored state: x = " << myObject.x << ", y = " << myObject.y << std::endl;

    // Restore the object to its saved state
    myObject.load(savedState2);
    
    // Output the restored object
	// Expected Output: "Restored state: x = 100, y = World"
    std::cout << "Restored state: x = " << myObject.x << ", y = " << myObject.y << std::endl;

    return 0;
}
