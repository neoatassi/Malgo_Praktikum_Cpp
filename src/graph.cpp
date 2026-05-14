#include "../include/graph.h"
#include <memory>
#include <vector>

using std::pair;

    Graph::Graph(int nodeCount)
    {
        this->nodeCount = nodeCount;
        adjList.resize(nodeCount);
        //weightedAdjList.resize(nodeCount);

        nodes.reserve(nodeCount);
        for (int i = 0; i < nodeCount; ++i)
            nodes.emplace_back(std::make_unique<Node>(i));
    }


    void UndirectedGraph::addEdge(int src, int dest, double weight)
    {
        // adjList[src].push_back(dest);
        // adjList[dest].push_back(src);

        this->getNode(src)->addNeighbor(getNode(dest), weight);
        this->getNode(dest)->addNeighbor(getNode(src), weight);

        edges.push_back({src, dest, weight});
    }

    void DirectedGraph::addEdge(int src, int dest, double weight)
    {
        adjList[src].push_back(dest);

        this->getNode(src)->addNeighbor(getNode(dest), weight);

        edges.push_back({src, dest, weight});
    }

    Node* Graph::getNode(int id) const
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

    // const std::vector<std::unique_ptr<Edge>>& Graph::getEdges() const
    // {
    //     return edges;
    // }

    const vector<Edge>& Graph::getEdges() const
    {
        return edges;
    }

    int Graph::getCount() const
    {
        return getNodes().size();
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





Node::Node (int id) : id(id) {}

const int& Node::getID() const{
    return id;
}

void Node::addNeighbor(Node* node, double weight)
{
    neighbors.push_back({node, weight});
}

const vector<pair<Node*, double>>& Node::getNeighbors() const
{
    return neighbors;
}

const pair<Node*, double>& Node::getNextNeighbor() const
{
    return neighbors.back();
}

void Node::popNeighbor(){
    neighbors.pop_back();        
}