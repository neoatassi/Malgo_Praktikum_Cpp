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
    std::vector<bool> visited;
    
    public:
    Graph(int vertices);

    void addEdge(int src, int dest);

    void reset();

    // auto& getAdjList();

    // auto& getVisited();

    // int getCount();

    void DFS (int vertex);

    void BFS(int vertex);
    
    int components();
    int mod_components(void (Graph::*traverseFn)(int));
    void printGraph();
};