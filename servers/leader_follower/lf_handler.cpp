#include "lf_handler.hpp"

LFHandler::LFHandler(unsigned int num_threads) : _running(true), _leader_id(0), _num_threads(num_threads) {
    run();
    for (unsigned int i = 0; i < num_threads; i++) {
        _threads.emplace_back(&LFHandler::worker_thread, this, i);
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

void LFHandler::worker_thread(unsigned int id) {
    while (true) {
        TaskFunction task;
        {
            std::unique_lock<std::mutex> lock(_tasks_mutex);
            _tasks_cv.wait(lock, [this, id] { return !_running || (!_tasks.empty() && _leader_id == id); });

            if (!_running && _tasks.empty()) return;
            if (_tasks.empty()) continue;

            task = std::move(_tasks.front());
            _tasks.pop();
            _leader_id = (_leader_id + 1) % _num_threads;
            _tasks_cv.notify_one();
        }

        if (task) task();
    }
}