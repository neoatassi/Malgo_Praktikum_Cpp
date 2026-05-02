#include "../include/prim.h"

double prim(const Graph& graph)
{
    int n = graph.getCount();
    const std::vector<std::vector<std::pair<int, double>>>& adjList = graph.getWeightedAdjList();

    // min-heap
    using Edge = std::pair<double, int>;
    std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> minHeap;

    std::vector<bool> visited(n, false);

    double totalWeight = 0.0;
    int edgesAdded = 0;

    // start from node 0
    minHeap.push({0.0, 0});

    while (!minHeap.empty() && edgesAdded < n) {
        auto [weight, vertex] = minHeap.top();
        minHeap.pop();

        // skip if already in MST
        if (visited[vertex]) continue;

        visited[vertex] = true;
        totalWeight += weight;
        edgesAdded++;

        for (auto& [neighbor, edgeWeight] : adjList[vertex]) {
            if (!visited[neighbor]) {
                minHeap.push({edgeWeight, neighbor});
            }
        }
    }

    return totalWeight;
}