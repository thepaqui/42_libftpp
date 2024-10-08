#include <iostream>
#include <iomanip>
#include <thread>
#include "thread_safe_iostream.hpp"

void printNumbers(const std::string& p_prefix) {
    threadSafeCout.setPrefix(p_prefix);
    for (int i = 1; i <= 5; ++i) {
        threadSafeCout << "Number: " << i << std::endl;
    }
}

void printStuff(const std::string& p_prefix) {
    threadSafeCout.setPrefix(p_prefix);
    threadSafeCout << std::right << std::showbase << std::hex;
    for (int i = 1; i <= 32; ++i) {
        if (i > 29)
            threadSafeCout << std::resetiosflags(std::ios_base::basefield);
        threadSafeCout
            << "Number: "
            << std::setw(20)
            << std::setfill('.')
            << i
            << std::endl
        ;
    }
    int intInput;
    threadSafeCout << "Enter hex number: " << std::flush;
    threadSafeCout >> std::hex >> intInput;
    threadSafeCout << "You entered hex number: " << intInput << std::endl;

	// If this test is different from real std::cin, it is
	// because of mutexes, as they prevent concurrent calls to
	// std::cin, and setw resets to 0 when >> is called for a string
    std::string strInput;
    threadSafeCout << "Enter string: " << std::flush;
    threadSafeCout >> std::setw(5) >> strInput;
    threadSafeCout << "You entered string: \'" << strInput << "\'" << std::endl;
}

int main() {
    std::string prefix1 = "[Thread 1] ";
    std::string prefix2 = "[Thread 2] ";

    std::thread thread1(printNumbers, prefix1);
    std::thread thread2(printNumbers, prefix2);

    thread1.join();
    thread2.join();

    return 0;
}
