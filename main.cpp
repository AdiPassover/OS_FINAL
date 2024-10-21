#include <iostream>
#include "servers/lf_server.hpp"

#include "servers/leader_follower/lf_handler.hpp"

int main(int argc, char* argv[]) {
    std::cout << "Hello world" << std::endl;

    LFHandler lf(5);
    for (int i = 0; i < 10; i++) {
        lf.add_task([i]() {
            std::cout << "Task " + std::to_string(i) + " started\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "\033[32mTask " + std::to_string(i) + " executed\033[0m\n";
        });
    }

    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "3 seconds passed\n";

    return 0; // temp
    char server_type;
    if (argc == 1) {
        std::cout << "No arguments provided, starting server with leader follower" << std::endl;
        server_type = 'l';
    } else if (argc != 2) {
        std::cerr << "Usage: -l for leader follower or -p for pipeline (and only one at a time)" << std::endl;
        return 1;
    } else {
        server_type = argv[1][1];
    }


    if (server_type == 'l') {
        std::cout << "Leader follower" << std::endl;
//        LeaderFollowerServer servers;
//        servers.run();
        run_lf_server();
    } else if (server_type == 'p') {
        std::cout << "Pipeline" << std::endl;
//        PipelineServer servers;
//        servers.run();
    } else {
        std::cerr << "Usage: -l for leader follower or -p for pipeline (and only one at a time)" << std::endl;
        return 1;
    }
}