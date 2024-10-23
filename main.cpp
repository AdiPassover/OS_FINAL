#include <iostream>
#include <csignal>
#include "servers/server.hpp"

std::unique_ptr<Server> server;

void handle_sigint(int sig) {
    std::cout << "\nReceived SIGINT (" << sig << "), shutting down server..." << std::endl;
    server->stop();
}

#include "servers/leader_follower/lf_handler.hpp"

int main(int argc, char* argv[]) {
    signal(SIGINT, handle_sigint);

    char server_type;
    if (argc == 1) {
        std::cout << "No arguments provided, starting server with leader follower" << std::endl;
        server_type = 'l';
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