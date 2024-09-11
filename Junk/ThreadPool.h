#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>
#include <stdexcept>
#include <iostream>

class ThreadPool {
public:
// Constructor to initialize the thread pool with a given number of threads
ThreadPool(size_t numThreads);

//Destructor to clean up the pool and ensure all tasks are completed
~ThreadPool() = default;

//     // Enqueues a task to be executed by the thread pool
//     void EnqueueTask(std::function<void()> task);
//     size_t GetActiveTaskCount() const;

// private:
//     // Function executed by each worker thread
//     void WorkerThread();

//     // Helper RAII class to manage active task counting
//     struct ActiveTaskCounter {
//         std::atomic<size_t>& activeTasks;
        
//         ActiveTaskCounter(std::atomic<size_t>& counter) : activeTasks(counter) {
//             activeTasks++;
//         }

//         ~ActiveTaskCounter() {
//             activeTasks--;
//         }
//     };

//     // Vector of worker threads
//     std::vector<std::thread> workers;

//     // Queue of tasks to be executed
//     std::queue<std::function<void()>> tasks;

//     // Synchronization tools
//     std::mutex queueMutex;
//     std::condition_variable condition;

//     // Atomic flag to signal stopping the pool
//     std::atomic<bool> stop;

//     // Counter to track active tasks
//     std::atomic<size_t> activeTasks;
};

#endif // THREADPOOL_H
