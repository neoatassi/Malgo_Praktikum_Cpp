#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <string>
#include <chrono>
#include <queue>
#include <algorithm>

using std::vector, std::pair;

// Seperate struct for weighted edges for Kruskal
struct Weighted {
    int src, dest;
    double weight;
};

class Graph {
    private:
    int nodeCount;
    int edgeCount;
    vector<vector<int>> adjList;
    vector<vector<pair<int, double>>> weightedAdjList;
    vector<Weighted> edges; 

    public:
    Graph(int nodeCount);
    Graph(int nodeCount, int edgeCount);

    void addEdge(int src, int dest, double weight);

    const vector<vector<int>>& getAdjList() const;
    const vector<vector<pair<int, double>>>& getWeightedAdjList() const;
    const vector<Weighted>& getEdges() const;

    int getCount() const;

    void printGraph();
};