#include "graph.hpp"
#include <stdexcept>

bool Edge::operator<(const Edge &other) const {
    return _weight < other._weight;
}

Edge::Edge(unsigned int u, unsigned int v, int weight) : _u(u), _v(v), _weight(weight) {}

Graph::Graph(unsigned int n) : _num_vertices(n) {
    _adj_matrix.resize(_num_vertices);
    for (unsigned int i = 0; i < _num_vertices; i++)
        _adj_matrix.resize(_num_vertices);
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
