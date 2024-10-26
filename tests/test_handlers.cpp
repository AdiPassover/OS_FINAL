#include "doctest.h"
#include "../servers/leader_follower/lf_handler.hpp"
#include "../servers/pipeline/pipeline_handler.hpp"

TEST_CASE("Leader follower") {
    LFHandler lf(5);
    std::atomic<int> tasks_done = 0;
    for (int i = 0; i < 10; i++) {
        lf.add_task([i, &tasks_done]() {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            tasks_done.fetch_add(1, std::memory_order_relaxed);
        });
    }
    std::this_thread::sleep_for(std::chrono::seconds(3));
    CHECK(tasks_done == 10);
}

TEST_CASE("Pipeline") {
    PipelineHandler pipelineHandler;
    std::atomic<int> tasks_done = 0;
    pipelineHandler.add_consecutive_stages("1",{
        [] (const std::string& str, int num) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            return str + std::to_string(num);
        },
        [] (const std::string& str, int num) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::string ans = str;
            for (int i = 0; i < num; i++) ans.append(ans);
            return ans;
        },
        [&tasks_done] (const std::string& str, int num) { tasks_done.fetch_add(1, std::memory_order_relaxed); return str; }
    }).add_stage("2", [&tasks_done] (const std::string& str, int num) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::string ans = str;
        ans[0] = std::to_string(num)[0];
        tasks_done.fetch_add(1, std::memory_order_relaxed);
        return ans;
    }).add_consecutive_stages("3", {
        [] (const std::string& str, int num) { std::this_thread::sleep_for(std::chrono::seconds(1)); return str; },
        [] (const std::string& str, int num) { std::this_thread::sleep_for(std::chrono::seconds(1)); return str; },
        [] (const std::string& str, int num) { std::this_thread::sleep_for(std::chrono::seconds(1)); return str; },
        [&tasks_done] (const std::string& str, int num) { tasks_done.fetch_add(1, std::memory_order_relaxed); return str; }
    });

    pipelineHandler.run_pipeline("1", "hello");
    pipelineHandler.run_pipeline("2", "what is up");
    pipelineHandler.run_pipeline("3", "yo");
    pipelineHandler.run_pipeline("3", "goodbye");
    pipelineHandler.run_pipeline("3", "goodbye world");

    std::this_thread::sleep_for(std::chrono::seconds(6));

    CHECK(tasks_done.load() == 5);
}