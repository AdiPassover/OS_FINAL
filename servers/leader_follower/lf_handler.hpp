#ifndef OS_FINAL_LF_HANDLER_HPP
#define OS_FINAL_LF_HANDLER_HPP

#include <functional>
#include <queue>
#include <list>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

class LFHandler {
public:
    using TaskFunction = std::function<void()>;

    explicit LFHandler(unsigned int num_threads); // automatically runs the handler
    ~LFHandler();

    void add_task(const TaskFunction &task);
    void run();
    void stop();


private:
    void worker_thread(unsigned int id);

    std::queue<TaskFunction> _tasks;
    std::list<std::thread> _threads;
    std::mutex _tasks_mutex;
    std::condition_variable _tasks_cv;
//    std::atomic<bool> _running;
    bool _running;
    unsigned int _leader_id;
    unsigned int _num_threads;

};


#endif //OS_FINAL_LF_HANDLER_HPP
