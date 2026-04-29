

#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include <chrono>

#include "graph.h"
#include "tests.h"



Graph loadGraph(const std::string& filepath)
{
    std::ifstream file(filepath);
    if (!file) {
        throw std::runtime_error("Could not open file: " + filepath);
    }
 
    int count;
    file >> count;
 
    Graph graph(count);
    

    std::cout << "Reading from file..  " << "\n";
    int v1, v2;

    auto start = std::chrono::high_resolution_clock::now();

    while (file >> v1 >> v2) {
        graph.addEdge(v1, v2);
    }
 
    auto end = std::chrono::high_resolution_clock::now();

    file.close();
 
    std::cout << "Graph saved!" << "\n";

    double ms = std::chrono::duration<double, std::milli>(end - start).count();


    std::cout << filepath
              << " | nodes: " << count
              << " | reading time: " << ms << " ms\n";
    
    return graph;
}
 

 
int main(int argc, char* argv[])
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
 
    std::string filepath = "";
    void (Graph::*traverseFn)(int) = &Graph::DFS;
    std::string algoName = "DFS";
    bool testMode = false;
 
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--bfs") {
            traverseFn = &Graph::BFS;
            algoName = "BFS";
        } else if (arg == "--dfs") {
            traverseFn = &Graph::DFS;
            algoName = "DFS";
        } else if (arg == "--test") {
            testMode = true;
        } else {
            filepath = arg;
        }
    }
 
    if (testMode) {
        runTests();
        return 0;
    }
 
    
    filepath = argv[1];

    Graph graph = loadGraph(filepath);
    
    std::cout << graph.mod_components(traverseFn);
 
    return 0;
}