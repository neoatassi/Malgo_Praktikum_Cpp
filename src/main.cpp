

#include <iostream>
#include <fstream>
//#include <filesystem>
#include <vector>
#include <string>
#include <chrono>

#include "../include/graph.h"
#include "../include/components.h"
//#include "../include/tests.h"



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

    // Disables cout buffering. Prints out statements on the go rather than after program terminates
    std::cout.setf(std::ios::unitbuf);

    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
 
    std::string filepath = "";
    void (*traverseFn)(const Graph&, int, std::vector<char>&) = DFS;
    std::string algoName = "DFS";
    bool testMode = false;
 
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--bfs") {
            traverseFn = BFS;
            algoName = "BFS";
        } else if (arg == "--dfs") {
            traverseFn = DFS;
            algoName = "DFS";
        } else if (arg == "--test") {
            testMode = true;
        } else {
            filepath = arg;
        }
    }
 
    if (testMode) {
        // runTests();
        return 0;
    }
 
    
    filepath = argv[1];

    Graph graph = loadGraph(filepath);
    
    //int components = graph.mod_components(traverseFn);

    int components = mod_components(graph, traverseFn);

    std::cout << components;
 
    return 0;
}