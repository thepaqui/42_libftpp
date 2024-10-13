#include "worker_pool.hpp"
#include <iostream>
#include <memory>
#include <chrono>
#include "../../iostream/iostream.hpp"

class IntJob : public WorkerPool::IJob {
private :
    int nb{0};

public :
    IntJob(int n) : nb(n) {};
    void execute() override {
        threadSafeCout << "Executing int job " << nb << " on thread " << std::this_thread::get_id() << std::endl;
    };
};

int main() {
    WorkerPool pool(4);

    auto job = []() {
        threadSafeCout << "Executing job on thread: " << std::this_thread::get_id() << std::endl;
    };

    for (int i = 0; i < 1000; ++i) {
        pool.addJob(job);
        if (i % 5 == 0) {
            auto job2 = std::make_shared<IntJob>(i);
            pool.addJob(job2);
        }
    }

    std::this_thread::sleep_for(std::chrono::seconds(2)); // Wait for jobs to finish

    return 0;
}
