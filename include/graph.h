#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <string>
#include <chrono>
#include <queue>
#include <algorithm>

class Graph {
    private:
    int count;
    std::vector<std::vector<int>> adjList;
    // defaults to false
    //std::vector<bool> visited;
    
    public:
    Graph(int vertices);

    void addEdge(int src, int dest);

    // void reset();

    const std::vector<std::vector<int>>& getAdjList() const;

    // auto& getVisited();

    // int getCount();

    void DFS (int vertex, std::vector<char>& visited);

    void BFS(int vertex, std::vector<char>& visited);
    
    int components();
    int mod_components(void (Graph::*traverseFn)(int, std::vector<char>&));
    void printGraph();
};