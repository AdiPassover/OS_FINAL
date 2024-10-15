#ifndef OS_FINAL_FIBONACCI_HEAP_HPP
#define OS_FINAL_FIBONACCI_HEAP_HPP

#include <vector>
#include <unordered_map>
#include <climits>

class FibonacciHeap { // TODO maybe switch to template
private:
    struct Node {
        const unsigned int _vertex;
        Node* _parent;
        Node* _left;
        Node* _right;
        Node* _child;
        int _key;
        unsigned int _degree;
        bool _is_marked;

        inline explicit  Node(unsigned int vertex, int key = INT_MAX) : _vertex(vertex), _key(key), _degree(0), _is_marked(false),
                                                       _parent(nullptr), _left(this), _right(this), _child(nullptr) {}
        ~Node();
    };

public:
    FibonacciHeap();
    ~FibonacciHeap();

    void insert(unsigned int vertex);
    unsigned int delete_min();
    void decrease_key(unsigned int vertex, int new_key);
    bool contains(unsigned int vertex) const;
    bool is_empty() const;

private:
    std::unordered_map<unsigned int, Node*> _nodes;
    Node* _min;
    unsigned int _size;
    std::vector<Node*> _consolidation_array;

    void consolidate();

    void insert_next_to_min(Node* node);
    void promote_child(Node* child);

};


#endif //OS_FINAL_FIBONACCI_HEAP_HPP
