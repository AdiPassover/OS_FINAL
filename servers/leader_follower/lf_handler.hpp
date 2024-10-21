#ifndef OS_FINAL_LF_HANDLER_HPP
#define OS_FINAL_LF_HANDLER_HPP

#include <functional>
#include <queue>
#include <list>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

typedef std::function<void()> TaskFunction;

class LFHandler {
public:
    explicit LFHandler(unsigned int num_threads);
    ~LFHandler();

    void add_task(const TaskFunction &task);
    void add_tasks(const std::initializer_list<TaskFunction>& tasks);
    void run();
    void stop();


private:
    void worker_thread();

    std::queue<TaskFunction> _tasks;
    std::list<std::thread> _threads;
    std::mutex _tasks_mutex;
    std::condition_variable _tasks_cv;
    std::atomic<bool> _running;
//    bool _running;
//    std::mutex _running_mutex;

};


#endif //OS_FINAL_LF_HANDLER_HPP
