#include "../include/graph.h"
#include "../include/debug.h"

#pragma once

struct MSTResult {
    std::vector<Edge> edges;
    double weight = 0.0;

    MSTResult (int edgeCount){
        edges.reserve(edgeCount);
    }
};

MSTResult kruskal (const Graph& graph); 