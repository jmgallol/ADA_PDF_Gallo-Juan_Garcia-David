#ifndef KRUSKAL_HPP
#define KRUSKAL_HPP

#include <vector>
#include "request.hpp"

struct Edge {
    int u, v;
    int cost;
    bool operator<(const Edge& other) const {
        if (cost != other.cost) return cost < other.cost;
        if (u != other.u) return u < other.u;
        return v < other.v;
    }
};

void runModuleB(const std::vector<Request>& requests);

#endif
