#include "../include/tsp.h"
#include <cstdlib>
#include <limits>
#include <utility>
#include <vector>

using std::vector, std::stack;

void rotateTourToStart(TourResult& result, int startId)
{
    // find node 0 in the tour (excluding the closing node)
    auto it = std::find_if(result.tour.begin(), result.tour.end() - 1,
        [startId](Node* n) { return n->getID() == startId; });

    if (it == result.tour.end() - 1) return;

    // rotate so node 0 is first
    std::rotate(result.tour.begin(), it, result.tour.end() - 1);

    // re-close the tour
    result.tour.back() = result.tour.front();
}

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

TourResult nearestNeighborBest (const Graph& graph)
{
    int nodeCount = graph.getCount();

    TourResult best(nodeCount);
    best.totalDistance = std::numeric_limits<double>::infinity();

    for (int start = 0; start < nodeCount; ++start){
        TourResult candidate = nearestNeighbor(graph, start);
        if (candidate.totalDistance < best.totalDistance)
            best = candidate;

    }

    rotateTourToStart(best, 0);

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

// ======================== Complete Search + Branch and Bound =================================

// helper function to find the lower bound
double lowerBound(SearchState& state, int current)
{
    double bound = 0.0;
    int n = state.graph.getCount();

    // calculate minimum cost of traversing remaining unvisited nodes
    // by getting the cheapest edge connected to each node and summing them
    for (int i = 0; i < n; ++i) {
        if (state.visited[i] == '0') {
            bound += state.graph.getNode(i)->getCheapestEdgeWeight();
        }
    }

    // calculate the cheapest way out of this node
    double minFromCurrent = std::numeric_limits<double>::infinity();
    for (auto& [neighbor, weight] : state.graph.getNode(current)->getNeighbors()) {
        if (state.visited[neighbor->getID()] == '0' && weight < minFromCurrent) {
            minFromCurrent = weight;
        }
    }

    if (minFromCurrent != std::numeric_limits<double>::infinity()) {
        bound += minFromCurrent;
    }

    return bound;
}

void searchRecursive(SearchState& state, int current, bool useBounds)
{
    int n = state.graph.getCount();

    // all nodes visited — close the tour
    if ((int)state.currentTour.size() == n) {
        double returnWeight = state.graph.getEdgeWeight(current, 0);
        double totalDistance = state.currentDistance + returnWeight;

        if (totalDistance < state.best.totalDistance) {
            state.best.totalDistance = totalDistance;
            state.best.tour.clear();
            for (int id : state.currentTour) {
                state.best.tour.push_back(state.graph.getNode(id));
            }
            state.best.tour.push_back(state.graph.getNode(0));
            debugLog("New best: " + std::to_string(totalDistance));
        }
        return;
    }

    for (auto& [neighbor, weight] : state.graph.getNode(current)->getNeighbors()) {
        int nextId = neighbor->getID();

        if (state.visited[nextId] == '1') continue;

        double newDistance = state.currentDistance + weight;

        // use lower bound to prune if needed
        if (useBounds) {
            // mark as visited before computing bound so it's excluded
            state.visited[nextId] = '1';
            double bound = newDistance + lowerBound(state, nextId);
            state.visited[nextId] = '0';

            if (bound >= state.best.totalDistance) continue; // prune
        } else {
            if (newDistance >= state.best.totalDistance) continue;
        }

        // recurse
        state.visited[nextId] = '1';
        state.currentTour.push_back(nextId);
        state.currentDistance = newDistance;

        searchRecursive(state, nextId, useBounds);

        // backtrack
        state.visited[nextId] = '0';
        state.currentTour.pop_back();
        state.currentDistance -= weight;
    }
}

TourResult completeSearch(const Graph& graph, int maxNodes)
{
    // Guarding clause to not compute large graphs
    if (graph.getCount() > maxNodes) return TourResult(graph.getCount());

    TourResult initial = nearestNeighborBest(graph);
    // TourResult initial = nearestNeighbor(graph, 0);
    SearchState state(graph, initial);

    state.visited[0] = '1';
    state.currentTour.push_back(0);

    searchRecursive(state, 0, false);

    return state.best;
}

TourResult branchAndBound(const Graph& graph, int maxNodes)
{
    // Guarding clause to not compute large graphs
    if (graph.getCount() > maxNodes) return TourResult(graph.getCount());

    TourResult initial = nearestNeighborBest(graph);
    // TourResult initial = nearestNeighbor(graph, 0);
    SearchState state(graph, initial);

    state.visited[0] = '1';
    state.currentTour.push_back(0);

    searchRecursive(state, 0, true);

    return state.best;
}