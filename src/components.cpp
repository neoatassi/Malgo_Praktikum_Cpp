#include "../include/components.h"


using std::vector;

void DFS (const Graph& graph, int vertex, std::vector<char>& visited)
    {
        //std::vector<bool> visited (this->count);
        //visited[vertex] = true;    
        
        const vector<vector<int>>& adjList = graph.getAdjList();

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

    void BFS(const Graph& graph, int vertex, std::vector<char>& visited)
    {

        //std::vector<bool> visited (this->count);

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
        }
    }

    int mod_components(const Graph& graph, void (*traverseFn)(const Graph&, int, std::vector<char>&))
    {

        const int& vcount = graph.getCount();

        std::vector<char> visited (vcount, '0');

        std::cout << "Processing..  " << "\n";
        auto start = std::chrono::high_resolution_clock::now();
        int components = 0;
        for (int i = 0; i < vcount; ++i) {
            if (visited[i] != '1') {
                (*traverseFn)(graph, i, visited);
                components++;
            }
        }
        std::cout << "Done!" << "\n";

        auto end = std::chrono::high_resolution_clock::now();
        double ms = std::chrono::duration<double, std::milli>(end - start).count();
        std::cout << "Processed in " << ms << " ms" << "\n";

        return components;
    }