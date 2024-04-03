#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
using namespace std;

// Class that represents a simple thread pool
class ThreadPool
{
public:
    // // Constructor to creates a thread pool with given
    // number of threads
    ThreadPool(size_t num_threads = thread::hardware_concurrency());

    // Destructor to stop the thread pool
    ~ThreadPool();

    // Enqueue task for execution by the thread pool
    void enqueue(function<void()> task);

    bool isQueueEmpty();

private:
    // Vector to store worker threads
    vector<thread> threads_;

    // Queue of tasks
    queue<function<void()>> tasks_;

    // Mutex to synchronize access to shared data
    mutex queue_mutex_;

    // Condition variable to signal changes in the state of
    // the tasks queue
    condition_variable cv_;

    // Flag to indicate whether the thread pool should stop
    // or not
    bool stop_ = false;
};