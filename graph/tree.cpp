#include "tree.hpp"

Tree::Tree(const std::list<Edge> &edges) {

}

Tree::Tree(const std::vector<unsigned int> &parents) {

}

int Tree::get_weight() const {
    return 0;
}

int Tree::get_shortest_distance() const {
    int min = _dist_matrix[0][1];
    for (unsigned int i = 0; i < _dist_matrix.size() - 1; i++) {
        for (unsigned int j = i+1; j < _dist_matrix[i].size(); j++) {
            min = std::min(min, _dist_matrix[i][j]);
        }
    }
    return min;
}

double Tree::get_avg_distance() const {
    int sum = 0;
    for (unsigned int i = 0; i < _dist_matrix.size() - 1; i++) {
        for (unsigned int j = i+1; j < _dist_matrix[i].size(); j++) {
            sum += _dist_matrix[i][j];
        }
    }
    return (double)sum / (double)(_dist_matrix.size() * (_dist_matrix.size() - 1) / 2);
}

int Tree::get_longest_distance() const {
    int max = _dist_matrix[0][1];
    for (unsigned int i = 0; i < _dist_matrix.size() - 1; i++) {
        for (unsigned int j = i+1; j < _dist_matrix[i].size(); j++) {
            max = std::max(max, _dist_matrix[i][j]);
        }
    }
    return max;
}
