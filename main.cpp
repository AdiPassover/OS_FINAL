#include <iostream>
#include <csignal>
#include "servers/server.hpp"

std::unique_ptr<Server> server = nullptr;

void handle_sigint(int sig) {
    std::cout << "\nReceived SIGINT (" << sig << "), shutting down server..." << std::endl;
    if (server != nullptr) server->stop();
}

int main(int argc, char* argv[]) {
    signal(SIGINT, handle_sigint);

    char server_type;
    if (argc == 1) {
        while (true) {
            std::cout << "No arguments provided, write [l] for leader follower or [p] for pipeline:" << std::endl;
            if (std::cin >> server_type) break;
        }
    } else if (argc == 2) {
        server_type = argv[1][1];
    } else {
        std::cerr << "Usage: -l for leader follower or -p for pipeline (and only one at a time)" << std::endl;
        return 1;
    }

    std::cout << "Creating server..." << std::endl;
    try {
        server = ServerFactory::get_server(server_type);
    } catch (const std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    server->run();

    return 0;
}