#include "../include/graph.h"

    Graph::Graph(int vertices)
    {
        count = vertices;
        adjList.resize(vertices);
        //visited.resize(vertices);
    }

    void Graph::addEdge(int src, int dest)
    {
        adjList[src].push_back(dest);
        adjList[dest].push_back(src);
    }

    const std::vector<std::vector<int>>& Graph::getAdjList() const
    {
        return adjList;
    }

    int Graph::getCount() const
    {
        return count;
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



