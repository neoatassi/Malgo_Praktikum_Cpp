#include "../include/tsp.h"
#include <cstdlib>
#include <limits>
#include <utility>
#include <vector>

using std::vector, std::stack;

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

vector<Node*> findEulerianCircuit(const Graph& graph) {
    stack<Node*> currPath;
    vector<Node*> circuit;
    
    // Start from vertex 0 (or any vertex with non-zero degree)
    // Ensure the graph is valid (all vertices have even degree in undirected, 
    // or equal in/out degree in directed) before calling this.
    Node* startNode = graph.getNode(0);
    currPath.push(startNode);
    
    while (!currPath.empty()) {
        auto node = currPath.top();
        
        // If there are remaining edges from u
        if (!node->getNeighbors().empty()) {
            // Get the next vertex
            auto nextNode = node->getNextNeighbor();
            node->popNeighbor(); // Remove the edge (u, v)
            currPath.push(nextNode.first);  // Move to v
        } else {
            // Backtrack: no more edges from u
            circuit.push_back(node);
            currPath.pop();
        }
    }
    
    // The circuit is constructed in reverse order
    reverse(circuit.begin(), circuit.end());
    return circuit;
}

TourResult doubleTree (const Graph& graph){

    int nodeCount = graph.getCount();

    TourResult result(nodeCount);

    // 1: Find MST
    MSTResult mst = kruskal(graph);

    // 2: Make an eulerisch graph (double the edges)

    DirectedGraph euGraph(nodeCount);
    
    for (auto& edge : mst.edges){
        euGraph.addEdge(edge.src, edge.dest, edge.weight);
        euGraph.addEdge(edge.dest, edge.src, edge.weight);

    }

    debugLog("Euler Graph edge count is: " + std::to_string(euGraph.getEdges().size()));

    // for (auto edge : euGraph.getEdges()){
    //     std::cout << edge.src << "\t->\t" << edge.dest << "\t" << edge.weight << "\n";
    // }

    // 3: find eulerian cicruit
    auto circuit = findEulerianCircuit(euGraph);

    // for (auto element : circuit){
    //     std::cout << element->getID() << "\t->\t" << edge.dest << "\t" << edge.weight << "\n";
    // }

    // // Print out euclerian circuit
    // std::for_each(circuit.begin(), circuit.end() - 1, [](Node* node) {
    //         std::cout << node->getID() << " -> ";
    //     });
    // std::cout << circuit.back()->getID() << "\n";

    // 4: shorten the circuit
    vector<char> visited(nodeCount, '0');

    for (auto node : circuit) {
        int nodeID = node->getID();
        if (visited[nodeID] != '1') {
            visited[nodeID] = '1';
            result.tour.push_back(graph.getNode(nodeID));
        }
    }

    // close the tour
    result.tour.push_back(result.tour[0]);

    // calculate total distance from original graph
    for (int i = 0; i < (int)result.tour.size() - 1; ++i) {
        Node* from = result.tour[i];
        Node* to = result.tour[i + 1];
        for (auto& [neighbor, weight] : from->getNeighbors()) {
            if (neighbor->getID() == to->getID()) {
                result.totalDistance += weight;
                break;
            }
        }
    }

    return result;
}