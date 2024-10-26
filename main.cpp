#include <iostream>
#include <csignal>
#include "servers/server.hpp"

std::unique_ptr<Server> server;

void handle_sigint(int sig) {
    std::cout << "\nReceived SIGINT (" << sig << "), shutting down server..." << std::endl;
    server->stop();
}

int main(int argc, char* argv[]) { // TODO test mst on non connected graphs
    signal(SIGINT, handle_sigint);

    char server_type;
    if (argc == 1) {
        while (true) {
            std::cout << "No arguments provided, write [l] for leader follower or [p] for pipeline:" << std::endl;
            if (std::cin >> server_type && (server_type == 'l' || server_type == 'p')) break;
        }
    } else if (argc == 2) {
        server_type = argv[1][1];
    } else {
        std::cerr << "Usage: -l for leader follower or -p for pipeline (and only one at a time)" << std::endl;
        return 1;
    }

    std::cout << "Creating server..." << std::endl;
    server = ServerFactory::get_server(server_type);

    server->run();
}