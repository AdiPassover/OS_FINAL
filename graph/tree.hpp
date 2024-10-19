#ifndef OS_FINAL_TREE_HPP
#define OS_FINAL_TREE_HPP

#include "graph.hpp"
#include <list>

class Tree {
public:
    Tree(const std::list<Edge> &edges);
    Tree(const std::vector<unsigned int> &parents);

private:


};


#endif //OS_FINAL_TREE_HPP
