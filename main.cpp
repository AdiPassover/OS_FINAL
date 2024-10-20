#include <iostream>
#include "servers/lf_server.hpp"

int main(int argc, char* argv[]) {
    std::cout << "Hello world" << std::endl;
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