#ifndef OS_FINAL_SERVER_HPP
#define OS_FINAL_SERVER_HPP

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <mutex>
#include "../graph/graph.hpp"
#include "leader_follower/lf_handler.hpp"
#include "pipeline/pipeline_handler.hpp"

#define PORT "9034"

class Server {
public:
    Server();
    virtual ~Server();

    void run();
    void stop();

protected:
    bool _running;
    int _listener;
    std::vector<struct pollfd> _poll_fds;
    std::unordered_map<int, Graph> _client_graphs;
    std::mutex _graphs_mutex;

    inline static constexpr unsigned int BUFFER_SIZE = 1024;
    inline static constexpr unsigned int MAX_CONNECTIONS = 10;

    virtual std::string handle_message(const std::string &command, int sender_fd);
    static void *get_in_addr(struct sockaddr *sa);
    static int get_listener_socket();

};

class LFServer : public Server {
public:
    LFServer(unsigned int num_threads);

protected:
    LFHandler _handler;

    std::string handle_message(const std::string &command, int sender_fd) override;
};

class PipelineServer : public Server {
public:
    PipelineServer();

protected:
    PipelineHandler _handler;

    std::string handle_message(const std::string &command, int sender_fd) override;
};

class ServerFactory {
public:
    static std::unique_ptr<Server> get_server(char type);
};

#endif //OS_FINAL_SERVER_HPP
