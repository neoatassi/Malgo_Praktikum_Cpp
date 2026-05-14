#include "../include/nearest_neighbor.h"
#include <limits>
#include <utility>
#include <vector>


TourResult nearestNeighbor(const Graph& graph, int start)
{
    int nodeCount = graph.getCount();

    // starting node
    //int startingIndex = rand() % graph.getCount();
    int startingIndex = start;
    auto startingNode = graph.getNode(startingIndex);


    auto currentNode = startingNode;

    // n nodes + return to start
    TourResult result(nodeCount);

    result.addNode(currentNode);

    vector<char> visited(nodeCount, '0');
    visited[currentNode->getID()] = '1';

    for (int step = 0; step < nodeCount - 1; ++step){
        Node* nearest = nullptr;
        double minWeight = std::numeric_limits<double>::infinity();

        for (auto& [neighbor, weight] : currentNode->getNeighbors()){
            if (visited[neighbor->getID()] != '1' && weight < minWeight){
                minWeight = weight;
                nearest = neighbor;
            }
        }

        visited[nearest->getID()] = '1';
        result.addNode(nearest);
        result.totalDistance += minWeight;
        currentNode = nearest;
    }

    for (auto& [neighbor, weight] : currentNode->getNeighbors()){
        if (neighbor == startingNode){
            result.totalDistance += weight;
            break;
        }
    }

    // bool done = false;
    // double minWeight = -1;
    // double totalWeight = 0;

    // Node* closest;
    

    // for (auto& node : graph.getNodes()){
    //     if (done) break;

    //     visited[node->getID()] = '1';

    //     for (auto& neighbor : node->getNeighbors()){
    //         if (visited[neighbor->dest->getID()] == '1') continue;
            
    //         if (minWeight == -1 || minWeight > neighbor->weight){
    //             minWeight = neighbor->weight;
    //             closest = neighbor->dest;
    //         }
            
    //         //visited[neighbor->dest->getID()] = '1';
    //     }

    //     totalWeight += minWeight;
    //     tour.push_back(closest);
    //     minWeight = -1;
    // }

    // for (auto& neighbor : closest->getNeighbors()){
    //     if (neighbor->dest == startingNode)
    //         totalWeight += neighbor->weight;
    // }

    //tour.push_back(startingNode);
    // return {tour, totalWeight};

    result.addNode(startingNode);
    return result;
    //return {result.tour, result.totalDistance};
}

TourResult bestTour (const Graph& graph)
{
    int nodeCount = graph.getCount();

    TourResult best(nodeCount);
    best.totalDistance = std::numeric_limits<double>::infinity();

    for (int start = 0; start < nodeCount; ++start){
        TourResult candidate = nearestNeighbor(graph, start);
        if (candidate.totalDistance < best.totalDistance)
            best = candidate;

    }

    return best;
}