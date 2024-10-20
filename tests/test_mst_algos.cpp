#include "doctest.h"
#include "../graph/mst_algo/MST_factory.hpp"

TEST_CASE("Simple") {
    std::unique_ptr<MST_algorithm> kruskal = MST_factory::get_algorithm(MST_factory::KRUSKAL);
    std::unique_ptr<MST_algorithm> prim = MST_factory::get_algorithm(MST_factory::PRIM);

    Graph g(5);
    g.add_edge(0,1,3); // <-
    g.add_edge(0,2,6);
    g.add_edge(0,4,7);
    g.add_edge(1,2,2); // <-
    g.add_edge(1,3,5);
    g.add_edge(1,4,6);
    g.add_edge(2,3,8);
    g.add_edge(2,4,2); // <-
    g.add_edge(3,4,4); // <-

    SUBCASE("kruskal") {
        Tree t_kruskal = kruskal->find_MST(g);
        CHECK(t_kruskal.get_weight() == 11);
        t_kruskal.get_avg_distance();
    }

    SUBCASE("prim") {
        Tree t_prim = prim->find_MST(g);
        CHECK(t_prim.get_weight() == 11);
    }

}