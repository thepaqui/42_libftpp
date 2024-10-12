#include <iostream>
#include <thread>
#include "thread_safe_queue.hpp"
#include "../../iostream/iostream.hpp"

void testPush(const std::string& prefix, ThreadSafeQueue<int>& p_queue, int p_value) {
    threadSafeCout.setPrefix(prefix);
    p_queue.push_front(p_value);
    threadSafeCout << "Pushed value: " << p_value << std::endl;
}

void testPop(const std::string& prefix, ThreadSafeQueue<int>& p_queue) {
    threadSafeCout.setPrefix(prefix);
    try {
        int value = p_queue.pop_back();
        threadSafeCout << "Popped value: " << value << std::endl;
    } catch (const std::runtime_error& e) {
        threadSafeCout << e.what() << std::endl;
    }
}

int main() {
    ThreadSafeQueue<int> myQueue;

    std::thread thread1(testPush, "[Thread 1] ", std::ref(myQueue), 42);
    std::thread thread2(testPush, "[Thread 2] ", std::ref(myQueue), 69);
    std::thread thread3(testPop, "[Thread 3] ", std::ref(myQueue));
    std::thread thread4(testPop, "[Thread 4] ", std::ref(myQueue));
    std::thread thread5(testPop, "[Thread 5] ", std::ref(myQueue));

    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();
    thread5.join();

    return 0;
}

