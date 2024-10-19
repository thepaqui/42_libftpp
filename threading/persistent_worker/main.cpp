#include "persistent_worker.hpp"
#include "../../iostream/iostream.hpp"
#include <iostream>
#include <chrono>

int main() {
    PersistentWorker worker;

    auto task1 = []() {
        threadSafeCout << "Executing Task 1" << std::endl;
    };

    auto task2 = []() {
        threadSafeCout << "Executing Task 2" << std::endl;
    };

    auto task3 = []() {
        threadSafeCout << "Executing Task 3" << std::endl;
    };

    worker.addTask("Task1", task1);
    worker.addTask("Task2", task2);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    worker.removeTask("Task1");
    worker.addTask("Task3", task3);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    worker.removeTask("Task2");
    worker.removeTask("Task3");

    std::this_thread::sleep_for(std::chrono::seconds(1));

    worker.addTask("Task1", task1);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    return 0;
}
