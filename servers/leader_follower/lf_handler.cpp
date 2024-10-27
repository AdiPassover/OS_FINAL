#include "lf_handler.hpp"

LFHandler::LFHandler(unsigned int num_threads) : _running(true) {
    run();
    for (unsigned int i = 0; i < num_threads; i++) {
        _threads.emplace_back(&LFHandler::worker_thread, this);
    }
}

LFHandler::~LFHandler() { stop(); }

void LFHandler::add_task(const TaskFunction &task) {
    {
        std::lock_guard<std::mutex> lock(_tasks_mutex);
        _tasks.push(task);
    }
    _tasks_cv.notify_one();
}

void LFHandler::add_tasks(const std::initializer_list<TaskFunction> &tasks) {
    {
        std::lock_guard<std::mutex> lock(_tasks_mutex);
        for (const auto &task : tasks)
            _tasks.push(task);
    }
    _tasks_cv.notify_one();
}

void LFHandler::run() {
    _running = true;
}

void LFHandler::stop() {
    {
        std::lock_guard<std::mutex> lock(_tasks_mutex);
        _running = false;
    }
    _tasks_cv.notify_all();

    for (auto &thread : _threads) {
        if (thread.joinable()) thread.join();
    }
}

void LFHandler::worker_thread() {
    while (true) {
        TaskFunction task;
        {
            std::unique_lock<std::mutex> lock(_tasks_mutex);
            _tasks_cv.wait(lock, [this] { return !_tasks.empty() || !_running; });

            if (!_running && _tasks.empty()) return;
            if (!_tasks.empty()) {
                task = std::move(_tasks.front());
                _tasks.pop();
            }
        }
        if (task) task();
    }
}