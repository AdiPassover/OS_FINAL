#ifndef OS_FINAL_MST_ALGORITHM_HPP
#define OS_FINAL_MST_ALGORITHM_HPP

#include "tree.hpp"

class MST_algorithm {
public:
    virtual Tree get_MST(const Graph&) const = 0;
    virtual ~MST_algorithm() = default;
};

class Kruskal : public MST_algorithm {
    virtual Tree get_MST(const Graph&) const override;
};

class Prim : public MST_algorithm {
    virtual Tree get_MST(const Graph&) const override;
};


#endif //OS_FINAL_MST_ALGORITHM_HPP
