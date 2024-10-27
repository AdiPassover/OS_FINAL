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
    lf.add_tasks({[]() {}, []() {}, []() {}});
    std::this_thread::sleep_for(std::chrono::seconds(3));
    CHECK(tasks_done == 10);
}