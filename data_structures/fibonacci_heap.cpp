#include "fibonacci_heap.hpp"
#include <stdexcept>

#include <iostream> // TODO remove

FibonacciHeap::FibonacciHeap() : _size(0), _min(nullptr), _nodes(), _node_of_degree(1) {}

FibonacciHeap::~FibonacciHeap() {
    std::cout << "deleting heap" << std::endl;
    if (_min != nullptr) {
        Node* current = _min;
        do {
            Node* temp = current;
            current = current->_right;
            temp->free_children();
            delete temp;
        } while (current != _min);
    }
}
void FibonacciHeap::insert(unsigned int vertex) {
    Node* new_node = new Node(vertex);
    if (_min == nullptr)
        _node_of_degree[0] = _min = new_node;
    else {
        insert_left_to_min(new_node);
        if (_node_of_degree[0] == nullptr)
            _node_of_degree[0] = new_node;
    }
    _nodes[vertex] = new_node;
    _size++;
}

void FibonacciHeap::insert_left_to_node(FibonacciHeap::Node *inserted_node, FibonacciHeap::Node *node) {
    node->_left->_right = inserted_node;
    inserted_node->_left = node->_left;
    inserted_node->_right = node;
    node->_left = inserted_node;
}

void FibonacciHeap::insert_left_to_min(FibonacciHeap::Node *node) {
    node->_is_marked = false;
    node->_parent = nullptr;
    insert_left_to_node(node, _min);
    if (node->_key < _min->_key)
        _min = node;
}

unsigned int FibonacciHeap::delete_min() {
    if (_min == nullptr) throw std::logic_error("Heap is empty");

    // Make min's children new roots
    std::cout << "promoting min's children" << std::endl;
    if (_min->_degree == 1) {
        insert_left_to_min(_min->_child);
    } else if (_min->_degree > 1) {
        Node* child = _min->_child;
        child->_left->_right = nullptr;
        while (child != nullptr) {
            Node* next = child->_right;
            insert_left_to_min(child);
            child = next;
        }
    }

    // Delete min
    std::cout << "deleting min" << std::endl;
    unsigned int min_vertex = _min->_vertex;
    Node* right = _min->_right;
    if (right != _min) {
        Node* left = _min->_left;
        right->_left = left;
        left->_right = right;
    }
    _nodes.erase(_min->_vertex);
    _min->_child = nullptr;
    if (_node_of_degree[_min->_degree] == _min)
        _node_of_degree[_min->_degree] = nullptr;
    delete _min;
    _size--;

    std::cout << "consolidating" << std::endl;
    if (_size != 0)
        consolidate(right);
    return min_vertex;
}

void FibonacciHeap::promote_child(FibonacciHeap::Node *child) {
    if (child->_key < _min->_key) _min = child;
    if (child->_parent == nullptr) return;
    Node* parent = child->_parent;

    // Remove child from parent's children and then insert it as a new root
    Node* right = child->_right;
    Node* left = child->_left;
    right->_left = left;
    left->_right = right;
    insert_left_to_min(child);

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
    if (new_key < _min->_key) _min = node;
    if (node->_parent == nullptr || node->_parent->_key <= new_key) return;
    promote_child(node);
}

void FibonacciHeap::consolidate(Node* start) {
    _min = start;
    Node* current = start;

    while (true) {
        std::cout << "current: " << current->_vertex << std::endl;
        for (unsigned int i = 0; i < _node_of_degree.size(); i++) {
            if (_node_of_degree[i] != nullptr) {
                std::cout << "degree " << i << ": " << _node_of_degree[i]->_vertex << " ";
            }
        } std::cout << std::endl;
        if (current->_key < _min->_key)
            _min = current;
        Node* next = current->_right;

        unsigned int degree = current->_degree;
        if (_node_of_degree.size() <= degree) {std::cout<<"resizing"<<std::endl; _node_of_degree.resize((degree+1)*2);}
        if (_node_of_degree[degree] == nullptr) {
            _node_of_degree[degree] = current;
        } else if (_node_of_degree[degree] != current) {
            next = merge(_node_of_degree[degree], current);
            if (_node_of_degree.size() <= next->_degree) {std::cout<<"resizing"<<std::endl; _node_of_degree.resize((next->_degree+1)*2);}
            if (_node_of_degree[next->_degree] == nullptr)
                _node_of_degree[next->_degree] = next;
            _node_of_degree[degree] = nullptr;
        }

        current = next;
        if (current == start) break;
    }
}

bool FibonacciHeap::empty() const {
    return _size == 0;
}

bool FibonacciHeap::contains(unsigned int vertex) const {
    return _nodes.find(vertex) != _nodes.end();
}

FibonacciHeap::Node* FibonacciHeap::merge(FibonacciHeap::Node *node1, FibonacciHeap::Node *node2) {
    Node* parent_node = (node1->_key < node2->_key) ? node1 : node2;
    Node* child_node = (parent_node == node1) ? node2 : node1;

    Node* child_left = child_node->_left;
    Node* child_right = child_node->_right;
    child_left->_right = child_right;
    child_right->_left = child_left;

    child_node->_parent = parent_node;
    if (parent_node->_child == nullptr) {
        parent_node->_child = child_node;
    } else {
        insert_left_to_node(child_node, parent_node->_child);
    }
    parent_node->_degree++;

    return parent_node;
}

unsigned int FibonacciHeap::min() const {
    if (_min == nullptr) throw std::logic_error("Heap is empty");
    return _min->_vertex;
}

FibonacciHeap::Node::~Node() {
    std::cout << "deleting node " << _vertex << std::endl;
    free_children();
}

void FibonacciHeap::Node::free_children() {
    if (_child != nullptr) {
        Node* child = _child;
        do {
            Node* next = child->_right;
            child->free_children();
            delete child;
            child = next;
        } while (child != _child);
    }
}
