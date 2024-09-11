// #include "ThreadPool.h"

// ThreadPool::ThreadPool(size_t numThreads) : stop(false), activeTasks(0) 
// {
//     for (size_t i = 0; i < numThreads; ++i) 
//     {
//         workers.emplace_back(&ThreadPool::WorkerThread, this);
//     }
// }

// ThreadPool::~ThreadPool() 
// {
//     // Signal all threads to stop
//     {
//         std::unique_lock<std::mutex> lock(queueMutex);
//         stop.store(true);
//     }
//     condition.notify_all();

//     // Join all threads
//     for (std::thread& worker : workers) 
//     {
//         if (worker.joinable()) {
//             worker.join();
//         }
//     }

//     // Wait for all active tasks to complete
//     while (activeTasks.load() > 0) {
//         std::this_thread::yield();
//     }

//     if (activeTasks.load() > 0) 
//     {
//         std::cerr << "Warning: Active tasks were not completed before shutdown." << std::endl;
//     }
// }

// size_t ThreadPool::GetActiveTaskCount() const 
// {
//     return activeTasks.load(); // Return the current value of the atomic counter
// }

// void ThreadPool::EnqueueTask(std::function<void()> task) 
// {
//     {
//         std::lock_guard<std::mutex> lock(queueMutex);
//         if (stop.load()) {
//             throw std::runtime_error("ThreadPool is shutting down, cannot enqueue tasks.");
//         }
        
//         // Wrap the task to handle the active task counting
//         tasks.push([this, task]() {
//             ActiveTaskCounter counter(activeTasks); // RAII for active task count
//             try {
//                 task();
//             } catch (...) {
//                 std::cerr << "Exception in task execution!" << std::endl;
//             }
//         });
//     }
//     condition.notify_one();
// }

// void ThreadPool::WorkerThread() 
// {
//     while (true) {
//         std::function<void()> task;
//         {
//             std::unique_lock<std::mutex> lock(queueMutex);

//             // Wait until there are tasks to process or the pool is stopping
//             condition.wait(lock, [this]() { return stop.load() || !tasks.empty(); });

//             if (stop.load() && tasks.empty()) {
//                 return;  // Exit thread if stopping and no tasks left
//             }

//             // Get the next task
//             task = std::move(tasks.front());
//             tasks.pop();
//         }
//         // Execute the task
//         task();
//     }
// }
