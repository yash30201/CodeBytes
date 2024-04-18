#include <thread>
#include <chrono>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>

std::mutex mtx;
std::condition_variable cv;
std::queue<int> task_queue;

void producer() {
    int x;
    while(true) {
        std::cout << "Do you want to add tasks? How many?: ";
        std::cin >> x;
        std::cout << std::endl;
        if (x == 13) break;
        for (int i = 0; i < x; ++i) {
            std::lock_guard<std::mutex> lock(mtx);
            task_queue.push(i);
            cv.notify_one();
        }
        std::cout << "Added " << x << "tasks" << std::endl;
    }
}

void consumer(int id) {
  while (true) {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, []{return !task_queue.empty();});
    int task = task_queue.front();
    task_queue.pop();
    std::cout << "Consumer " << id << " processing: " << task << std::endl;
    lock.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

int main() {
  const int num_consumers = 2;
  std::thread pool[num_consumers];

  std::thread producer_thread(producer);

  for (int i = 0; i < num_consumers; ++i) {
    pool[i] = std::thread([i](){
        consumer(i);
    });
  }

  for (int i = 0; i < num_consumers; ++i) {
    pool[i].detach();
  }
  producer_thread.join();


  return 0;
}
