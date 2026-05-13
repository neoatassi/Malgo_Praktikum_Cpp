#include "../include/graph.h"
#include <memory>
#include <vector>

    Graph::Graph(int nodeCount)
    {
        this->nodeCount = nodeCount;
        adjList.resize(nodeCount);
        //weightedAdjList.resize(nodeCount);

        nodes.reserve(nodeCount);
        for (int i = 0; i < nodeCount; ++i)
            nodes.emplace_back(std::make_unique<Node>(i));
    }

    // Graph::Graph(int nodeCount, int edgeCount)
    // {
    //     this->nodeCount = nodeCount;
    //     this->edgeCount = edgeCount;
    //     adjList.resize(nodeCount);
    //     weightedAdjList.resize(nodeCount);
    //     wEdges.reserve(edgeCount);
    //     //visited.resize(vertices);
    // }

    // void Graph::addEdge(int src, int dest, double weight)
    // {
    //     adjList[src].push_back(dest);
    //     adjList[dest].push_back(src);
        
    //     if (edgeCount != 0) {
    //         weightedAdjList[src].push_back({dest, weight});
    //         weightedAdjList[dest].push_back({src, weight});

    //         wEdges.push_back({src, dest, weight});
    //     }
    // }

    
    void UndirectedGraph::addEdge(int src, int dest, double weight)
    {

        if (src >= (int)nodes.size() || dest >= (int)nodes.size()) {
            throw std::runtime_error("Node index out of bounds");
        }

        adjList[src].push_back(dest);
        adjList[dest].push_back(src);

        // register new (forward) edge from src to dest
        Edge* fwdPtr = edges.emplace_back(std::make_unique<Edge>()).get();
        // register new (backward) edge from dest to src
        Edge* bwdPtr = edges.emplace_back(std::make_unique<Edge>()).get();

        // alias for src and dest nodes
        Node* srcNode = nodes[src].get();
        Node* destNode = nodes[dest].get();
        
        // set edge attributes
        fwdPtr->src = srcNode;
        fwdPtr->dest = destNode;
        fwdPtr->weight = weight;

        bwdPtr->src = destNode;
        bwdPtr->dest = srcNode;
        bwdPtr->weight = weight;

        // register the new edges in the neighbor lists for each node
        srcNode->setNeighbor(fwdPtr);
        destNode->setNeighbor(bwdPtr);
    }

    void DirectedGraph::addEdge(int src, int dest, double weight)
    {
        adjList[src].push_back(dest);

        // constructes an edge object and adds it to vector in on line
        Edge* fwdPtr = edges.emplace_back(std::make_unique<Edge>()).get();
        
        Node* srcNode = nodes[src].get();
        Node* destNode = nodes[dest].get();
        
        fwdPtr->src = srcNode;
        fwdPtr->dest = destNode;
        fwdPtr->weight = weight;

        srcNode->setNeighbor(fwdPtr);
    }

    const Node* Graph::getNode(int id) const
    {
        return nodes[id].get();
    }

    const vector<std::unique_ptr<Node>>& Graph::getNodes() const
    {
        return nodes;
    }

    const std::vector<std::vector<int>>& Graph::getAdjList() const
    {
        return this->adjList;
    }

    const vector<vector<pair<int, double>>>& Graph::getWeightedAdjList() const
    {
        return this->weightedAdjList;
    }

    const std::vector<Weighted>& Graph::getwEdges() const
    {
        return wEdges;
    }

    const std::vector<std::unique_ptr<Edge>>& Graph::getEdges() const
    {
        return edges;
    }

    int Graph::getCount() const
    {
        return this->nodeCount;
    }

    void Graph::printGraph()
    {
        for (int i = 0; i < (int)adjList.size(); ++i){

            std::cout << i << ": ";

            for (int neighbor : adjList[i]){
                std::cout << neighbor << " -> ";
            }

            std::cout << "NULL" << "\n";
        } 
    }



