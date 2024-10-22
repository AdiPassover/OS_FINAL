#ifndef OS_FINAL_PIPELINE_HANDLER_HPP
#define OS_FINAL_PIPELINE_HANDLER_HPP

#include <functional>
#include <unordered_map>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

class PipelineHandler {
private:
    using ProcessFunction = std::function<std::string(const std::string&, int)>;
public:
    PipelineHandler();
    ~PipelineHandler();

    PipelineHandler& add_stage(const std::string& key, const ProcessFunction& processFunc);
    PipelineHandler& add_consecutive_stages(const std::string& key, const std::initializer_list<ProcessFunction> &processFuncs);

    void run_pipeline(const std::string& key, const std::string& command, int sender_fd, const std::function<void(
            const std::string &)> &on_end_func);
    void stop();

private:
    class PipelineStage { // Active Object pattern
    public:
        using TaskFunction = std::function<void()>;

        explicit PipelineStage(const ProcessFunction& processFunction, const std::shared_ptr<PipelineStage>& next_stage = nullptr);
        ~PipelineStage();

        void run_stage(const std::string& command, int sender_fd, const std::function<void(
                const std::string &)> &on_end_func);
        void stop();

    private:
        void run();
        PipelineStage& add_task(TaskFunction task); // returns reference to itself for chaining

    private:
        std::thread _thread;
        std::atomic<bool> _running;
        std::queue<TaskFunction> _tasks;
        std::mutex _tasks_mutex;
        std::condition_variable _new_task_cv;

        std::shared_ptr<PipelineStage> _next_stage;
        ProcessFunction _process;
    };

private:
    std::atomic<bool> _running;
    std::unordered_map<std::string, std::shared_ptr<PipelineStage>> _pipelines;


};


#endif //OS_FINAL_PIPELINE_HANDLER_HPP