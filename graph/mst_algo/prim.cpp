#include "MST_algorithms.hpp"
#include "../../data_structures/fibonacci_heap.hpp"

void decrease_key(FibonacciHeap &heap, std::vector<int> &key, unsigned int u, int new_key) {
    heap.decrease_key(u, new_key);
    key[u] = new_key;
}

Tree Prim::find_MST(const Graph &graph) const {
    std::vector<int> key(graph.num_vertices(), INT_MAX);
    std::vector<unsigned int> parent(graph.num_vertices(), graph.num_vertices());
    std::vector<int> parent_weight(graph.num_vertices(), 0);
    FibonacciHeap heap;
    for (unsigned int i = 0; i < graph.num_vertices(); i++)
        heap.insert(i);
    decrease_key(heap, key, 0, 0);

    while (!heap.empty()) {
        unsigned int u = heap.delete_min();
        for (unsigned int v = 0; v < graph.num_vertices(); v++) {
            if (graph.has_edge(u, v) && heap.contains(v) && graph.get_weight(u, v) < key[v]) {
                decrease_key(heap, key, v, graph.get_weight(u, v));
                parent[v] = u;
                parent_weight[v] = graph.get_weight(u, v);
            }
        }
    }

    return Tree{parent, parent_weight};
}