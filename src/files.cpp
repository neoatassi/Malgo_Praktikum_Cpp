#include "../include/files.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <filesystem>
#include <chrono>

namespace fs = std::filesystem;

const std::string BASE_DIR = "../Graph";

const std::vector<std::string> SUBDIRS = {
    "Components",
    "MST"
};

std::vector<std::string> getGraphFiles(const std::string& directory)
{
    //std::string directory = "../Graph";
    std::vector<std::string> files;
    if (!fs::exists(directory) || !fs::is_directory(directory)) {
        std::cerr << "Directory " << directory << " not found.\n";
        return files;
    }
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension().string() == ".txt") {
            files.push_back(entry.path().string());
        }
    }
    //std::sort(files.begin(), files.end());
    return files;
}

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

std::unique_ptr<Graph> loadGraph(const std::string& filepath, const std::string& type)
{
    std::ifstream file(filepath);
    if (!file) {
        throw std::runtime_error("Could not open file: " + filepath);
    }
 
    int nodeCount, edgeCount = 0;
    file >> nodeCount;
    file.ignore();
 
    // if (filepath.find("MST") != std::string::npos) {
    //     debugLog("MST FILENAME CONVENTION DETECTED");
    //     nodeCount = parseFilename(filepath).first;
    //     edgeCount = parseFilename(filepath).second;
    // }
    
    // Graph graph(nodeCount, edgeCount);

    std::unique_ptr<Graph> graph;

    if (type == "UndirectedGraph")   graph = std::make_unique<UndirectedGraph>(nodeCount);
    else if (type == "DirectedGraph") graph = std::make_unique<DirectedGraph>(nodeCount);
    //else if (type == "flow")     graph = std::make_unique<FlowGraph>(nodeCount);
    else throw std::runtime_error("Unknown graph type: " + type);

    //std::cout << "Reading from file..  " << "\n";
    int v1, v2;
    double w = 0.0;

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

        graph->addEdge(v1, v2, w);
    } while (std::getline(file, line));

    debugLog("Edge count: " + std::to_string(graph->getEdges().size()) + "\n");

    debugLog("Node count: " + std::to_string(graph->getNodes().size()) + "\n");
 
    auto end = std::chrono::high_resolution_clock::now();

    file.close();
 
    //std::cout << "Graph saved!" << "\n";

    double ms = std::chrono::duration<double, std::milli>(end - start).count();


    // std::cout << filepath
    //           << " | nodes: " << nodeCount
    //           << " | reading time: " << ms << " ms\n";
    
    return graph;
}