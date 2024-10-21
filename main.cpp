#include <iostream>
#include <csignal>
#include "servers/server.hpp"

std::unique_ptr<Server> server;

void handle_sigint(int sig) {
    std::cout << "\nReceived SIGINT (" << sig << "), shutting down servers..." << std::endl;
    server->stop();
}

int main(int argc, char* argv[]) {
    signal(SIGINT, handle_sigint);
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

    // TODO temporary!
    server = std::make_unique<Server>();
//    server = ServerFactory::get_server(server_type);

    server->run();
}