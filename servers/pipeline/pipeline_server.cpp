#include "../server.hpp"
#include "../../graph/mst_algo/MST_factory.hpp"
#include "../client_commands.hpp"

PipelineServer::PipelineServer() : Server(), _handler() {
    auto send_message_func = [this](const std::string &msg, int fd) {
        std::lock_guard<std::mutex> lock(_send_mutex);
        send_message(fd, msg);
        return std::string{};
    };

    _handler.add_consecutive_stages(ClientCommands::NEW_GRAPH_STR, {
            [this](const std::string& command, int fd) {
                std::vector<std::string> args = ClientCommands::split(command);
                Graph new_graph = ClientCommands::handle_new_graph(args);

                std::unique_lock<std::mutex> graphs_lock(_graphs_mutex);
                auto& [graph, graph_mutex] = _client_graphs[fd];
                graphs_lock.unlock();

                {
                    std::lock_guard<std::mutex> lock(*graph_mutex);
                    graph = new_graph;
                }
                return ClientCommands::NEW_GRAPH_RES;
            }, send_message_func

        }).add_consecutive_stages(ClientCommands::ADD_EDGE_STR, {
            [this](const std::string& command, int fd) {
                std::vector<std::string> args = ClientCommands::split(command);

                std::unique_lock<std::mutex> graphs_lock(_graphs_mutex);
                auto& [graph, graph_mutex] = _client_graphs[fd];
                graphs_lock.unlock();

                {
                    std::lock_guard<std::mutex> lock(*graph_mutex);
                    ClientCommands::handle_add_edge(args, graph);
                }
                return ClientCommands::NEW_GRAPH_RES;
            }, send_message_func

        }).add_consecutive_stages(ClientCommands::REMOVE_EDGE_STR, {
            [this](const std::string& command, int fd) {
                std::vector<std::string> args = ClientCommands::split(command);

                std::unique_lock<std::mutex> graphs_lock(_graphs_mutex);
                auto& [graph, graph_mutex] = _client_graphs[fd];
                graphs_lock.unlock();

                {
                    std::lock_guard<std::mutex> lock(*graph_mutex);
                    ClientCommands::handle_remove_edge(args, graph);
                }
                return ClientCommands::NEW_GRAPH_RES;
            }, send_message_func

        }).add_consecutive_stages(ClientCommands::MST_STR, {
            [this](const std::string& command, int fd) {
                std::vector<std::string> args = ClientCommands::split(command);
                if (args.size() != 2) throw std::invalid_argument("Invalid number of arguments for mst");
                std::unique_ptr<MST_algorithm> mst_algo = MST_factory::get_algorithm(args[1]);

                std::unique_lock<std::mutex> graphs_lock(_graphs_mutex);
                auto& [graph, graph_mutex] = _client_graphs[fd];
                graphs_lock.unlock();

                {
                    std::lock_guard<std::mutex> lock(*graph_mutex);
                    // TODO make a tree and pass it forward in the pipeline
                    Tree tree = mst_algo->get_MST(graph);
                }
                return ClientCommands::NEW_GRAPH_RES;
            }, send_message_func

        }).add_consecutive_stages(ClientCommands::PRINT_GRAPH_STR, {
            [this](const std::string& command, int fd) {
                std::vector<std::string> args = ClientCommands::split(command);

                std::unique_lock<std::mutex> graphs_lock(_graphs_mutex);
                auto& [graph, graph_mutex] = _client_graphs[fd];
                graphs_lock.unlock();
                std::string response;

                {
                    std::lock_guard<std::mutex> lock(*graph_mutex);
                    response = ClientCommands::handle_print_graph(graph);
                }
                return response;
            }, send_message_func

        }).add_consecutive_stages(ClientCommands::RANDOM_GRAPH_STR,{
            [this](const std::string& command, int fd) {
                std::vector<std::string> args = ClientCommands::split(command);

                std::unique_lock<std::mutex> graphs_lock(_graphs_mutex);
                auto& [graph, graph_mutex] = _client_graphs[fd];
                graphs_lock.unlock();
                std::string response;

                {
                    std::lock_guard<std::mutex> lock(*graph_mutex);
                    response = ClientCommands::handle_print_graph(graph);
                }
                return response;
            }, send_message_func

        }).add_stage(INVALID_KEY, send_message_func);
}

void PipelineServer::handle_message(const std::string &command, int sender_fd) {
    std::string command_key = command.substr(0, command.find(' '));

    if (ClientCommands::get_command(command_key) == ClientCommands::INVALID)
        _handler.run_pipeline(INVALID_KEY, ClientCommands::INVALID_COMMAND_RES, sender_fd);
    else
        _handler.run_pipeline(command_key, command, sender_fd);
}
