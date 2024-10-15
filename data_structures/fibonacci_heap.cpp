#include "fibonacci_heap.hpp"
#include <stdexcept>

FibonacciHeap::FibonacciHeap() : _size(0), _min(nullptr), _nodes(), _consolidation_array() {}

FibonacciHeap::~FibonacciHeap() {
    while (_min != nullptr) {
        Node* temp = _min;
        _min = _min->_right;
        delete temp;
    }
}

void FibonacciHeap::insert(unsigned int vertex) {
    Node* new_node = new Node(vertex);
    if (_min == nullptr)
        _min = new_node;
    else
        insert_next_to_min(new_node);
    _nodes[vertex] = new_node;
    _size++;
}

void FibonacciHeap::insert_next_to_min(FibonacciHeap::Node *node) {
    node->_is_marked = false;
    node->_parent = nullptr;
    _min->_left->_right = node;
    node->_left = _min->_left;
    node->_right = _min;
    _min->_left = node;
    if (node->_key < _min->_key)
        _min = node;
}

unsigned int FibonacciHeap::delete_min() {
    if (_min == nullptr) throw std::logic_error("Heap is empty");

    // Make min's children new roots
    if (_min->_degree == 1) {
        insert_next_to_min(_min->_child);
    } else if (_min->_degree > 1) {
        Node* child = _min->_child;
        child->_left->_right = nullptr;
        while (child != nullptr) {
            Node* next = child->_right;
            insert_next_to_min(child);
            child = next;
        }
    }

    // Delete min
    unsigned int min_vertex = _min->_vertex;
    if (_min->_right != _min) {
        Node* right = _min->_right;
        Node* left = _min->_left;
        right->_left = left;
        left->_right = right;
    }
    _nodes.erase(_min->_vertex);
    delete _min;
    _size--;

    consolidate();
    return min_vertex;
}

void FibonacciHeap::promote_child(FibonacciHeap::Node *child) {
    if (child->_parent == nullptr) return;
    Node* parent = child->_parent;

    // Remove child from parent's children and then insert it as a new root
    Node* right = child->_right;
    Node* left = child->_left;
    right->_left = left;
    left->_right = right;
    insert_next_to_min(child);

    // Update parent's degree and child
    parent->_degree--;
    if (parent->_degree == 0)
        parent->_child = nullptr;
    else if (parent->_child == child)
        parent->_child = right;

    if (!parent->_is_marked)
        parent->_is_marked = (parent->_parent != nullptr); // Mark parent if it's not a root
    else
        promote_child(parent);
}

void FibonacciHeap::decrease_key(unsigned int vertex, int new_key) {
    Node* node = _nodes[vertex];
    if (new_key > node->_key) throw std::logic_error("New key is greater than current key");
    node->_key = new_key;
    if (node->_parent == nullptr || node->_parent->_key <= new_key) return;
    promote_child(node);
}

void FibonacciHeap::consolidate() {
    // TODO remember that _min is now nullptr and I need to recalc it
}

bool FibonacciHeap::is_empty() const {
    return _size == 0;
}

bool FibonacciHeap::contains(unsigned int vertex) const {
    return _nodes.find(vertex) != _nodes.end();
}

FibonacciHeap::Node::~Node() {
    Node* child = _child;
    while (child != nullptr) {
        Node* next = child->_right;
        delete child;
        child = next;
    }
}
