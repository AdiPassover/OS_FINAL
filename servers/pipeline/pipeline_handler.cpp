#include "pipeline_handler.hpp"

PipelineHandler::PipelineStage::PipelineStage(const ProcessFunction& processFunction,
                                              const std::shared_ptr<PipelineStage>& next_stage)
    : _running(true), _next_stage(next_stage), _process(processFunction) {
    _thread = std::thread(&PipelineHandler::PipelineStage::run, this);
}

PipelineHandler::PipelineStage::~PipelineStage() { stop(); }

void PipelineHandler::PipelineStage::run() {
    while (true) {
        TaskFunction task;
        {
            std::unique_lock<std::mutex> lock(_tasks_mutex);
            _new_task_cv.wait(lock, [this] { return !_tasks.empty() || !_running; });

            if (!_running && _tasks.empty()) return;
            if (!_tasks.empty()) {
                task = std::move(_tasks.front());
                _tasks.pop();
            }
        }
        if (task) task();
    }
}

void PipelineHandler::PipelineStage::stop() {
    {
        std::lock_guard<std::mutex> lock(_tasks_mutex);
        _running = false;
        _new_task_cv.notify_all();
    }
    if (_thread.joinable()) _thread.join();
}

PipelineHandler::PipelineStage& PipelineHandler::PipelineStage::add_task(TaskFunction task) {
    {
        std::lock_guard<std::mutex> lock(_tasks_mutex);
        _tasks.push(std::move(task));
        _new_task_cv.notify_one();
    }
    return *this;
}

void PipelineHandler::PipelineStage::run_stage(const std::string &command, int sender_fd) {
    add_task([this, command, sender_fd] {
        std::string response;
        try {
            response = _process(command, sender_fd);
        } catch (const std::exception& ex) {
            response = ex.what();
        }
        if (_next_stage != nullptr) _next_stage->run_stage(response, sender_fd);
    });
}

// ===================

PipelineHandler::PipelineHandler() : _running(false), _pipelines() {}

PipelineHandler::~PipelineHandler() { stop(); }

PipelineHandler& PipelineHandler::add_stage(const std::string& key, const ProcessFunction& processFunc) {
    _pipelines[key] = std::make_shared<PipelineStage>(processFunc);
    return *this;
}

PipelineHandler& PipelineHandler::add_consecutive_stages(const std::string& key, const std::initializer_list<ProcessFunction> &processFuncs) {
    std::shared_ptr<PipelineStage> curr = nullptr;
    for (auto it = rbegin(processFuncs); it != rend(processFuncs); ++it) {
        curr = std::make_shared<PipelineStage>(*it, curr);
    }
    _pipelines[key] = curr;
    return *this;
}

void PipelineHandler::run_pipeline(const std::string &key, const std::string &command, int sender_fd) {
    _pipelines[key]->run_stage(command, sender_fd);
}

void PipelineHandler::stop() {
    for (auto& pair : _pipelines) {
        pair.second->stop();
    }
}