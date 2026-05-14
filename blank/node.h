#pragma once

#include <vector>

using std::vector;

class Node;

struct Edge {
    Node* src;
    Node* dest;
    double weight;
};

class Node {
    private:
    int id;
    vector<Edge*> neighbors;
    
    public:
    Node (int id);

    const int& getID() const;
    void setNeighbor(Edge* edge);
    const vector<Edge*>& getNeighbors() const;

};