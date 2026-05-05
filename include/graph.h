#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <string>
#include <chrono>
#include <queue>
#include <algorithm>
#include <memory>

#include "../include/node.h"

using std::vector, std::pair;

// Seperate struct for weighted edges for Kruskal
struct Weighted {
    int src, dest;
    double weight;
};

class Graph {
    protected:
    int nodeCount;
    int edgeCount;
    vector<vector<int>> adjList;
    vector<vector<pair<int, double>>> weightedAdjList;
    vector<Weighted> wEdges;
    vector<std::unique_ptr<Node>> nodes;
    vector<std::unique_ptr<Edge>> edges;

    public:
    Graph(int nodeCount);
    //Graph(int nodeCount, int edgeCount);

    virtual void addEdge(int src, int dest, double weight) = 0;

    const vector<vector<int>>& getAdjList() const;
    const vector<vector<pair<int, double>>>& getWeightedAdjList() const;
    const vector<Weighted>& getwEdges() const;
    const vector<std::unique_ptr<Edge>>& getEdges() const;
    const Node* getNode(int id) const;
    const vector<std::unique_ptr<Node>>& getNodes() const;

    int getCount() const;

    void printGraph();
    virtual ~Graph() = default;
};

class UndirectedGraph : public Graph {
    public:
    UndirectedGraph(int n) : Graph(n) {}
    void addEdge(int src, int dest, double weight) override;
};

class DirectedGraph : public Graph {
    public:
    DirectedGraph(int n) : Graph(n) {}
    void addEdge(int src, int dest, double weight) override;
};