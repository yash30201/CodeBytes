#include <chrono>
#include <thread>
#include <iostream>

auto t = std::chrono::high_resolution_clock::now();
void time_wrapper_call(void (*func)()) {
    std::cout << "Entered at: " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - t).count() << std::endl;
    func();
    std::cout << "Exiting at : " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - t).count() << std::endl;
}

class SimpleThread {
    static std::chrono::time_point<std::chrono::steady_clock> t;
    static void worker_impl() {
        std::cout << "Worker function executed" << std::endl;
    }
    static void worker() {
        time_wrapper_call(worker_impl);
    }
public:
    void run() {
        std::thread t1(worker);
        // t1.detach();
        t1.join();
        std::cout << "Ran everything" << std::endl;
        return;
    }
};

int main() {
    SimpleThread byte;
    byte.run();
    return 0;
}

