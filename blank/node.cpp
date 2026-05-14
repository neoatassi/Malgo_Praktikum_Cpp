#include "../include/node.h"

using std::pair;

Node::Node (int id) : id(id) {}

const int& Node::getID() const{
    return id;
}

void Node::setNeighbor(Edge* edge)
{
    this->neighbors.push_back(edge);
}

const vector<Edge*>& Node::getNeighbors() const
{
    //vector<pair<int,double>> neighborList;
    //
    //for (auto adj : this->neighbors){
    //    neighborList.push_back({adj->dest->id,adj->weight});
    //}

    return neighbors;
}