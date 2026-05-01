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

    public:
    Graph(int vertices);

    void addEdge(int src, int dest);

    const std::vector<std::vector<int>>& getAdjList() const;

    int getCount() const;

    void printGraph();
};