#include "../include/graph.h"
#include <vector>

    Graph::Graph(int nodeCount)
    {
        this->nodeCount = nodeCount;
        adjList.resize(nodeCount);
        //weightedAdjList.resize(nodeCount);
    }

    Graph::Graph(int nodeCount, int edgeCount)
    {
        this->nodeCount = nodeCount;
        this->edgeCount = edgeCount;
        adjList.resize(nodeCount);
        weightedAdjList.resize(nodeCount);
        edges.reserve(edgeCount);
        //visited.resize(vertices);
    }

    void Graph::addEdge(int src, int dest, double weight)
    {
        adjList[src].push_back(dest);
        adjList[dest].push_back(src);
        
        if (edgeCount != 0) {
            weightedAdjList[src].push_back({dest, weight});
            weightedAdjList[dest].push_back({src, weight});

            edges.push_back({src, dest, weight});
        }
    }

    const std::vector<std::vector<int>>& Graph::getAdjList() const
    {
        return this->adjList;
    }

    const vector<vector<pair<int, double>>>& Graph::getWeightedAdjList() const
    {
        return this->weightedAdjList;
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



