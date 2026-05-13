#include "../include/nearest_neighbor.h"

pair<vector<Node*>, double> nearestNeighbor(const Graph& graph)
{
    vector<Node*> tour;

    int nodeCount = graph.getCount();
    auto startingNode = graph.getNodes()[0].get();

    // n nodes + return to start
    tour.reserve(nodeCount + 1);
    tour.push_back(startingNode);

    vector<char> visited(graph.getCount(), '0');
    visited[startingNode->getID()] = '1';

    bool done = false;
    double minWeight = -1;
    double totalWeight = 0;

    Node* closest;
    

    for (auto& node : graph.getNodes()){
        if (done) break;

        visited[node->getID()] = '1';

        for (auto& neighbor : node->getNeighbors()){
            if (visited[neighbor->dest->getID()] == '1') continue;
            
            if (minWeight == -1 || minWeight > neighbor->weight){
                minWeight = neighbor->weight;
                closest = neighbor->dest;
            }
            
            //visited[neighbor->dest->getID()] = '1';
        }

        totalWeight += minWeight;
        tour.push_back(closest);
        minWeight = -1;
    }

    for (auto& neighbor : closest->getNeighbors()){
        if (neighbor->dest == startingNode)
            totalWeight += neighbor->weight;
    }

    tour.push_back(startingNode);
    return {tour, totalWeight};
}