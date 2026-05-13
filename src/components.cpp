#include "../include/components.h"


using std::vector;

    void DFS (const Graph& graph, int vertex, std::vector<char>& visited)
    {
        
        const vector<vector<int>>& adjList = graph.getAdjList();

        std::stack<int> stack;
        stack.push(vertex);

        visited[vertex] = '1';

        while (!stack.empty()){

            vertex = stack.top();
            stack.pop();

            // for (int neighbor : adjList[vertex]){
            //     if (visited[neighbor] != '1') {
            //         //std::cout << "\n";
            //         stack.push(neighbor);
            //         visited[neighbor] = '1';
            //     }
            // }

            for (auto& neighbor : graph.getNode(vertex)->getNeighbors()){
                
                int neighborID = neighbor->dest->getID();
                
                if(visited[neighborID] != '1'){
                    stack.push(neighborID);
                    visited[neighborID] = '1';
                }
            }
        }
    }

    void BFS(const Graph& graph, int vertex, std::vector<char>& visited)
    {

        const vector<vector<int>>& adjList = graph.getAdjList();

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

            // for (auto& neighbor : graph.getNode(vertex)->getNeighbors()){
                
            //     const int& neighborID = neighbor->dest->getID();
                
            //     if(visited[neighborID] != '1'){
            //         queue.push(neighborID);
            //         visited[neighborID] = '1';
            //     }
            // }

        }
    }

    int countComponents(const Graph& graph, void (*traverseFn)(const Graph&, int, std::vector<char>&))
    {

        const int& vcount = graph.getCount();

        std::vector<char> visited (vcount, '0');

        debugLog("Processing..  ");
        auto start = std::chrono::high_resolution_clock::now();
        int components = 0;
        for (int i = 0; i < vcount; ++i) {
            if (visited[i] != '1') {
                (*traverseFn)(graph, i, visited);
                components++;
            }
        }
        debugLog("Done!");

        auto end = std::chrono::high_resolution_clock::now();
        double ms = std::chrono::duration<double, std::milli>(end - start).count();
        debugLog("Processed in " + std::to_string(ms) + " ms");

        return components;
    }