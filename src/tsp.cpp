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
    TourResult result(nodeCount + 1);

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
    
    // Start from node 0
    Node* startNode = graph.getNode(0);
    currPath.push(startNode);
    
    while (!currPath.empty()) {
        auto node = currPath.top();
        
        // If there are remaining neighboring edges
        if (!node->getNeighbors().empty()) {
            // Get the next node and remove it from the neighbor's list
            auto nextNode = node->getNextNeighbor();
            node->popNeighbor();
            currPath.push(nextNode.first);  // Move to the next node
        } else {
            // Backtrack
            circuit.push_back(node);
            currPath.pop();
        }
    }
    
    // circuit is constructed in reverse order
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

    // Print out euclerian circuit
    // if (CIRCUIT_PRINT) {
    //     std::cout << "Euler Circuit:" << "\t";
    //     std::for_each(circuit.begin(), circuit.end() - 1, [](Node* node) {
    //         std::cout << node->getID() << " -> ";
    //     });
    //     std::cout << circuit.back()->getID() << "\n";
    // }
    

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