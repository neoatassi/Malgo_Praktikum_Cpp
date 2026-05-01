#include "../include/graph.h"

    Graph::Graph(int nodeCount)
    {
        this->nodeCount = nodeCount;
        adjList.resize(nodeCount);
        //visited.resize(vertices);
    }

    Graph::Graph(int nodeCount, int edgeCount)
    {
        this->nodeCount = nodeCount;
        this->edgeCount = edgeCount;
        adjList.resize(nodeCount);
        edges.reserve(edgeCount);
        //visited.resize(vertices);
    }

    void Graph::addEdge(int src, int dest, double weight)
    {
        adjList[src].push_back(dest);
        adjList[dest].push_back(src);
        
        if (edgeCount != 0)
            edges.push_back({src, dest, weight});
    }

    const std::vector<std::vector<int>>& Graph::getAdjList() const
    {
        return this->adjList;
    }

    const std::vector<Weighted>& Graph::getEdges() const
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



