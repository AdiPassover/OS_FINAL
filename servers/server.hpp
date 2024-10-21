#ifndef OS_FINAL_SERVER_HPP
#define OS_FINAL_SERVER_HPP

#include <string>
#include <vector>
#include <memory>

#define PORT "9034"

class Server {
public:
    explicit Server();
    ~Server();

    void run();
    void stop();

protected:
    bool _running;
    int _listener;
    std::vector<struct pollfd> _poll_fds;

    inline static constexpr unsigned int BUFFER_SIZE = 1024;
    inline static constexpr unsigned int MAX_CONNECTIONS = 10;

    std::string process_command(const std::string &command);
    static void *get_in_addr(struct sockaddr *sa);
    static int get_listener_socket();

};

class LFServer : public Server {
public:
    inline explicit LFServer() : Server() {}

};

class PipelineServer : public Server {
public:
    inline explicit PipelineServer() : Server() {}

};

class ServerFactory {
public:
    static std::unique_ptr<Server> get_server(char type);
};

#endif //OS_FINAL_SERVER_HPP
