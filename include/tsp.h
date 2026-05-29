#pragma once

#include "graph.h"
#include "kruskal.h"
#include "prim.h"
#include <vector>

using std::vector, std::pair;

struct TourResult {
    std::vector<Node*> tour;
    double totalDistance = 0.0;

    TourResult (int n) {
        // n nodes + return to start
        tour.reserve(n + 1);
    }

    void addNode (Node* node){
        tour.push_back(node);
    }
};

// shared recursive state to avoid copying on every call
struct SearchState {
    const Graph& graph;
    vector<char> visited;
    vector<int> currentTour;
    double currentDistance;
    TourResult best;

    SearchState(const Graph& graph, TourResult initialBest)
        : graph(graph),
          visited(graph.getCount(), '0'),
          currentDistance(0.0),
          best(std::move(initialBest)) // seeding with initialBest means best doesn't start at infinity
    {
        currentTour.reserve(graph.getCount() + 1);
    }
};

TourResult nearestNeighbor(const Graph& graph, int start);
TourResult nearestNeighborBest (const Graph& graph);
TourResult doubleTree (const Graph& graph);
TourResult completeSearch(const Graph& graph, int maxNodes = 12);
TourResult branchAndBound(const Graph& graph, int maxNodes = 12);