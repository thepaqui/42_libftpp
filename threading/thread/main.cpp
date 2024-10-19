#include "thread.hpp"
#include "../../iostream/iostream.hpp"
#include <iostream>
#include <chrono>

void myFunction1() {
    for (int i = 0; i < 5; ++i) {
        threadSafeCout << "Hello from Function1, iteration " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void myFunction2() {
    for (int i = 0; i < 5; ++i) {
        threadSafeCout << "Hello from Function2, iteration " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

int main() {
    Thread thread1("Thread1", myFunction1);
    Thread thread2("Thread2", myFunction2);
    try {
        Thread thread3("Thread3", nullptr);
    } catch (const std::exception& e) {
        threadSafeCout << "Caught exception: " << e.what() << std::endl;
    }

    thread1.start();
    thread2.start();

    thread1.stop();
    threadSafeCout << "Thread1 stopped." << std::endl;
    thread2.stop();
    threadSafeCout << "Thread2 stopped." << std::endl;

    return 0;
}
