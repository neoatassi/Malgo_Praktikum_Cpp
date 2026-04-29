// #include <iostream>
// #include <fstream>
// #include <vector>
// #include <stack>
// #include <string>
// #include <chrono>
// #include <queue>

#include "../include/graph.h"



    // private:
    // int count;
    // std::vector<std::vector<int>> adjList;
    // // defaults to false
    // std::vector<bool> visited;
    

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

    // void Graph::reset()
    // {
    //     std::fill(visited.begin(), visited.end(), false);
    // }

    const std::vector<std::vector<int>>& Graph::getAdjList() const
    {
        return adjList;
    }

    // auto& getVisited()
    // {
    //     return this->visited;
    // }

    // int getCount()
    // {
    //     return this->count;
    // }

    void Graph::DFS (int vertex, std::vector<char>& visited)
    {
        //std::vector<bool> visited (this->count);
        //visited[vertex] = true;    
        
        std::stack<int> stack;
        stack.push(vertex);

        visited[vertex] = '1';

        //std::cout << vertex << " ";
        while (!stack.empty()){

            vertex = stack.top();
            stack.pop();

            for (int neighbor : adjList[vertex]){
                if (visited[neighbor] != '1') {
                    //std::cout << "\n";
                    stack.push(neighbor);
                    visited[neighbor] = '1';
                }
            }
        }
    }

    void Graph::BFS(int vertex, std::vector<char>& visited)
    {

        //std::vector<bool> visited (this->count);

        std::queue<int> queue;
        queue.push(vertex);
        visited[vertex] = '1';
 
        //std::cout << vertex << " ";

        while (!queue.empty()) {

            vertex = queue.front();
            queue.pop();
 
            for (int neighbor : adjList[vertex]) {
                if (visited[neighbor] != '1') {
                    //std::cout << "\n";
                    queue.push(neighbor);
                    visited[neighbor] = '1';
                }
            }
        }
    }
    
    int Graph::components()
    {
        std::vector<char> visited (this->count);


        auto start = std::chrono::high_resolution_clock::now();

        int counter = 0;
        for (int i = 0; i < this->count; ++i)
        {
            if (!visited[i]){
                //DFS(i);
                BFS(i, visited);
                counter++;
            }
        }

        auto end = std::chrono::high_resolution_clock::now();
        double ms = std::chrono::duration<double, std::milli>(end - start).count();
        std::cout << "Processed in " << ms << " ms" << "\n";
        return counter;
    }

    int Graph::mod_components(void (Graph::*traverseFn)(int, std::vector<char>&))
    {

        std::vector<char> visited (this->count, '0');

        std::cout << "Processing..  " << "\n";
        auto start = std::chrono::high_resolution_clock::now();
        int counter = 0;
        for (int i = 0; i < count; ++i) {
            if (visited[i] != '1') {
                (this->*traverseFn)(i, visited);
                counter++;
            }
        }
        std::cout << "Done!" << "\n";

        auto end = std::chrono::high_resolution_clock::now();
        double ms = std::chrono::duration<double, std::milli>(end - start).count();
        std::cout << "Processed in " << ms << " ms" << "\n";

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



