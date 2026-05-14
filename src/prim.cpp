#include "../include/prim.h"

#include <cstdlib>

using std::vector, std::pair, std::priority_queue, std::greater;

double prim(const Graph& graph)
{
    int n = graph.getCount();
    //const vector<vector<pair<int, double>>>& adjList = graph.getWeightedAdjList();
    auto& adjList = graph.getEdges();

    // min-heap
    using Edge = std::pair<double, int>;
    // priority_queue<pair<double,int>,
    //             vector<pair<double,int>>,
    //             greater<pair<double,int>>> minHeap;

    priority_queue<pair<double,int>,
            vector<pair<double,int>>,
            greater<pair<double,int>>> minHeap;
    

    std::vector<bool> visited(n, false);

    double totalWeight = 0.0;
    int edgesAdded = 0;

    // start from node 0
    // hier aufpassen für P3
    int startNode = rand() % graph.getCount();
    //Edge startingNode = (adjList[startSeed][0].second, adjList[startSeed][0].first);
    // minHeap.push({0.0, 0});
    minHeap.push({0.0, startNode});

    while (!minHeap.empty()) {
        auto [weight, vertex] = minHeap.top();
        minHeap.pop();

        // skip if already in MST
        if (visited[vertex]) continue;

        visited[vertex] = true;
        totalWeight += weight;
        edgesAdded++;

        for (auto& [neighbor, weight] : graph.getNode(vertex)->getNeighbors()){
            if (!visited[neighbor->getID()]){
                minHeap.push({weight, neighbor->getID()});
            }
        }

        // for (auto& neighbor : graph.getNode(vertex)->getNeighbors()){
        //     if(!visited[neighbor->dest->getID()]){
        //         minHeap.push({neighbor->weight, neighbor->dest->getID()});
        //     }
        // }

        // for (auto& [neighbor, edgeWeight] : adjList[vertex]) {
        //     if (!visited[neighbor]) {
        //         minHeap.push({edgeWeight, neighbor});
        //     }
        // }
    }

    return totalWeight;
}

// double prim(const Graph& graph)
// {
//     int n = graph.getCount();
//     const std::vector<std::vector<std::pair<int, double>>>& adjList = graph.getWeightedAdjList();

//     // min-heap
//     using Edge = std::pair<double, int>;
//     std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> minHeap;

//     std::vector<bool> visited(n, false);

//     double totalWeight = 0.0;
//     int edgesAdded = 0;

//     // start from node 0
//     // hier aufpassen für P3
//     int startNode = rand() % graph.getCount();
//     //Edge startingNode = (adjList[startSeed][0].second, adjList[startSeed][0].first);
//     // minHeap.push({0.0, 0});
//     minHeap.push({0.0, startNode});

//     while (!minHeap.empty()) {
//         auto [weight, vertex] = minHeap.top();
//         minHeap.pop();

//         // skip if already in MST
//         if (visited[vertex]) continue;

//         visited[vertex] = true;
//         totalWeight += weight;
//         edgesAdded++;

//         for (auto& [neighbor, edgeWeight] : adjList[vertex]) {
//             if (!visited[neighbor]) {
//                 minHeap.push({edgeWeight, neighbor});
//             }
//         }
//     }

//     return totalWeight;
// }