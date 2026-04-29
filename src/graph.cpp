// #include <iostream>
// #include <fstream>
// #include <vector>
// #include <stack>
// #include <string>
// #include <chrono>
// #include <queue>

#include "graph.h"



    // private:
    // int count;
    // std::vector<std::vector<int>> adjList;
    // // defaults to false
    // std::vector<bool> visited;
    

    Graph::Graph(int vertices)
    {
        count = vertices;
        adjList.resize(vertices);
        visited.resize(vertices);
    }

    void Graph::addEdge(int src, int dest)
    {
        adjList[src].push_back(dest);
        adjList[dest].push_back(src);
    }

    void Graph::reset()
    {
        std::fill(visited.begin(), visited.end(), false);
    }

    // auto& getAdjList()
    // {
    //     return this->adjList;
    // }

    // auto& getVisited()
    // {
    //     return this->visited;
    // }

    // int getCount()
    // {
    //     return this->count;
    // }

    void Graph::DFS (int vertex)
    {
        //visited[vertex] = true;    
        
        std::stack<int> stack;
        stack.push(vertex);

        visited[vertex] = true;

        //std::cout << vertex << " ";
        while (!stack.empty()){

            vertex = stack.top();
            stack.pop();

            for (int neighbor : adjList[vertex]){
                if (!visited[neighbor]) {
                    //std::cout << "\n";
                    stack.push(neighbor);
                    visited[neighbor] = true;
                }
            }
        }
    }

    void Graph::BFS(int vertex)
    {
        std::queue<int> queue;
        queue.push(vertex);
        visited[vertex] = true;
 
        //std::cout << vertex << " ";

        while (!queue.empty()) {

            vertex = queue.front();
            queue.pop();
 
            for (int neighbor : adjList[vertex]) {
                if (!visited[neighbor]) {
                    //std::cout << "\n";
                    queue.push(neighbor);
                    visited[neighbor] = true;
                }
            }
        }
    }
    
    int Graph::components()
    {
        int counter = 0;
        for (int i = 0; i < this->count; ++i)
        {
            if (!visited[i]){
                //DFS(i);
                BFS(i);
                counter++;
            }
        }
        return counter;
    }

    int Graph::mod_components(void (Graph::*traverseFn)(int))
    {
        std::cout << "Processing..  " << "\n";
        int counter = 0;
        for (int i = 0; i < count; ++i) {
            if (!visited[i]) {
                (this->*traverseFn)(i);
                counter++;
            }
        }
        std::cout << "Done!" << "\n";
        return counter;
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



