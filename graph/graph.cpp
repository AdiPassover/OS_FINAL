#include "graph.hpp"
#include <stdexcept>
#include <sstream>
#include <queue>

bool Edge::operator<(const Edge &other) const {
    return _weight < other._weight;
}

Edge::Edge(unsigned int u, unsigned int v, int weight) : _u(u), _v(v), _weight(weight) {}

Graph::Graph(unsigned int n) : _num_vertices(n), _adj_matrix(n, std::vector<int>(n, NO_EDGE)) {}

Graph::Graph(unsigned int num_vertices, unsigned int num_edges, int seed) : Graph(num_vertices) {
    if (num_edges > (num_vertices * num_vertices - num_vertices) / 2)
        throw std::invalid_argument("Too many edges");
    srand(seed);

    std::vector<Edge> edges;
    edges.reserve((num_vertices * num_vertices - num_vertices) / 2);
    for (unsigned int i = 0; i < num_vertices - 1; i++) {
        for (unsigned int j = i+1; j < num_vertices; j++) {
            edges.emplace_back(i, j);
        }
    }

    while (edges.size() > num_edges) {
        unsigned int random_index = rand() % edges.size();
        std::swap(edges[random_index], edges.back()); // Swapping so we can use pop_back instead of erasing the mid
        edges.pop_back();
    }

    for (const auto& edge : edges)
        add_edge(edge);
}

void Graph::add_edge(unsigned int u, unsigned int v, int weight) {
    if (u == v) throw std::logic_error("Can't have an edge from a vertex to itself");
    _adj_matrix.at(u).at(v) = _adj_matrix.at(v).at(u) = weight;
}

void Graph::remove_edge(unsigned int u, unsigned int v) {
    _adj_matrix.at(u).at(v) = _adj_matrix.at(v).at(u) = NO_EDGE;
}

bool Graph::has_edge(unsigned int u, unsigned int v) const {
    return _adj_matrix.at(u).at(v) != NO_EDGE;
}

int Graph::get_weight(unsigned int u, unsigned int v) const {
    return _adj_matrix.at(u).at(v);
}

unsigned int Graph::num_vertices() const {
    return _num_vertices;
}

std::string Graph::to_string() const {
    std::stringstream ss;
    for (unsigned int i = 0; i < _num_vertices; i++) {
        for (unsigned int j = 0; j < _num_vertices; j++) {
            std::string weight = !has_edge(i, j) ? "X" : std::to_string(_adj_matrix[i][j]);
            ss << weight << " ";
        } ss << '\n';
    }
    return ss.str();
}

bool Graph::is_connected() const {
    std::queue<unsigned int> q;
    std::vector<bool> visited(_num_vertices, false);
    unsigned int count = 1;
    visited[0] = true;
    q.push(0);

    while (!q.empty()) {
        unsigned int curr = q.front();
        q.pop();

        for (unsigned int v = 0; v < _num_vertices; v++) {
            if (has_edge(curr, v) && !visited[v]) {
                count++;
                visited[v] = true;
                q.push(v);
            }
        }
    }

    return count == _num_vertices;
}
