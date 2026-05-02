

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <vector>
#include <string>
#include <chrono>

#include "../include/graph.h"
#include "../include/components.h"
#include "../include/kruskal.h"
//#include "../include/tests.h"

std::pair<int, int> parseFilename(const std::string& filepath)
{
    std::string filename = std::filesystem::path(filepath).stem().string(); // "G_1_200"
    
    std::istringstream iss(filename);
    std::string part;

    std::getline(iss, part, '_'); // "G"
    std::getline(iss, part, '_'); // "1"
    int nodes = std::stoi(part) * 1000;

    std::getline(iss, part, '_'); // "200"
    int edges = std::stoi(part) * 1000;

    return {nodes, edges};
}

Graph loadGraph(const std::string& filepath)
{
    std::ifstream file(filepath);
    if (!file) {
        throw std::runtime_error("Could not open file: " + filepath);
    }
 
    int nodeCount, edgeCount = 0;
    file >> nodeCount;
    file.ignore();
 
    if (filepath.find("MST") != std::string::npos) {
        std::cout << "MST DETECTED";
        nodeCount = parseFilename(filepath).first;
        edgeCount = parseFilename(filepath).second;
    }
    
    Graph graph(nodeCount, edgeCount);

    std::cout << "Reading from file..  " << "\n";
    int v1, v2;
    double w = 0;

    auto start = std::chrono::high_resolution_clock::now();

    // while (file >> v1 >> v2 >> w) {
    //     graph.addEdge(v1, v2, w);
    // }

    std::string line;
    do {
        if (line.empty()) continue;

        std::istringstream iss(line);
        iss >> v1 >> v2;
        iss >> w;

        graph.addEdge(v1, v2, w);
    } while (std::getline(file, line));
 
    auto end = std::chrono::high_resolution_clock::now();

    file.close();
 
    std::cout << "Graph saved!" << "\n";

    double ms = std::chrono::duration<double, std::milli>(end - start).count();


    std::cout << filepath
              << " | nodes: " << nodeCount
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

    // int components = mod_components(graph, traverseFn);
    //std::cout << components;
    
    double mstWeight = kruskal(graph);

    std::cout << "Total MST Weight: " << mstWeight << "\n";

    return 0;
}