#ifndef OS_FINAL_TREE_HPP
#define OS_FINAL_TREE_HPP

#include "graph.hpp"
#include <list>

class Tree {
public:
    Tree(const std::list<Edge> &edges);
    Tree(const std::vector<unsigned int> &parents);

    int get_weight() const;
    int get_shortest_distance() const;
    double get_avg_distance() const;
    int get_longest_distance() const;

private:
    const Graph _tree;
    std::vector<std::vector<int>> _dist_matrix;
    int _weight;

    void init_distances();
    void bfs(unsigned int src);

};


#endif //OS_FINAL_TREE_HPP
