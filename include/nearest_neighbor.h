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

TourResult nearestNeighbor(const Graph& graph, int start);
TourResult bestTour (const Graph& graph);