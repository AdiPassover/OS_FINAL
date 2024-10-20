#include "graph.hpp"
#include <stdexcept>

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
        unsigned int randomIndex = rand() % edges.size();
        std::swap(edges[randomIndex], edges.back()); // Swapping so we can use pop_back instead of erasing the mid
        edges.pop_back();
    }

    for (const auto& edge : edges)
        add_edge(edge);
}

void Graph::add_edge(unsigned int u, unsigned int v, int weight) {
    if (u == v) throw std::logic_error("Can't have an edge from a vertex to itself");
    _adj_matrix[u][v] = _adj_matrix[v][u] = weight;
}

void Graph::remove_edge(unsigned int u, unsigned int v) {
    _adj_matrix[u][v] = _adj_matrix[v][u] = NO_EDGE;
}

bool Graph::has_edge(unsigned int u, unsigned int v) const {
    return _adj_matrix[u][v] != NO_EDGE;
}

int Graph::get_weight(unsigned int u, unsigned int v) const {
    return _adj_matrix[u][v];
}

unsigned int Graph::num_vertices() const {
    return _num_vertices;
}