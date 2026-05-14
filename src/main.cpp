

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
#include "../include/prim.h"
#include "../include/files.h"
#include "../include/debug.h"
#include "../include/tests.h"
#include "../include/nearest_neighbor.h"

void printUsage(const char* programName)
{
    std::cerr << "Usage:\n"
              << "  " << programName << " --algo <dfs|bfs|kruskal|prim> --dir <directory>\n"
              << "  " << programName << " --algo <dfs|bfs|kruskal|prim> --file <filepath>\n"
              << "  " << programName << " --test\n";
}


 void processFile(const std::string& filepath, const std::string& algo)
{
    std::unique_ptr<Graph> graph;
    std::string filename = std::filesystem::path(filepath).filename().string();

    auto start = std::chrono::high_resolution_clock::now();

    if (algo == "dfs") {
        graph = loadGraph(filepath, "UndirectedGraph");
        int result = countComponents(*graph, &DFS);
        auto end = std::chrono::high_resolution_clock::now();
        double ms = std::chrono::duration<double, std::milli>(end - start).count();
        std::cout << "[DFS] " << filename << " | components: " << result
                  << " | time: " << ms << " ms\n";

    } else if (algo == "bfs") {
        graph = loadGraph(filepath, "UndirectedGraph");
        int result = countComponents(*graph, &BFS);
        auto end = std::chrono::high_resolution_clock::now();
        double ms = std::chrono::duration<double, std::milli>(end - start).count();
        std::cout << "[BFS] " << filename << " | components: " << result
                  << " | time: " << ms << " ms\n";

    } else if (algo == "kruskal") {
        graph = loadGraph(filepath, "UndirectedGraph");
        double result = kruskal(*graph);
        auto end = std::chrono::high_resolution_clock::now();
        double ms = std::chrono::duration<double, std::milli>(end - start).count();
        std::cout << "[Kruskal] " << filename << " | MST weight: " << result
                  << " | time: " << ms << " ms\n";

    } else if (algo == "prim") {
        graph = loadGraph(filepath, "UndirectedGraph");
        double result = prim(*graph);
        auto end = std::chrono::high_resolution_clock::now();
        double ms = std::chrono::duration<double, std::milli>(end - start).count();
        std::cout << "[PRIM] " << filename << " | MST weight: " << result
                  << " | time: " << ms << " ms\n";
    // } else if (algo == "nn" ) {
    //     graph = loadGraph(filepath, "UndirectedGraph");
    //     auto result = nearestNeighbor(*graph);
    //     auto end = std::chrono::high_resolution_clock::now();
    //     double ms = std::chrono::duration<double, std::milli>(end - start).count();
    //     std::cout << "[NEAREST NEIGHBOR] " << filename << " | Optimal tour: " << result.second
    //               << " | time: " << ms << " ms\n";

    //     for (auto& node : result.first){
    //         std::cout << node->getID() << " -> ";
    //     }
    } else {
        std::cerr << "Unknown algorithm: " << algo << "\n";
    }
}
 
int main(int argc, char* argv[])
{

    // Disables cout buffering. Prints out statements on the go rather than after program terminates
    std::cout.setf(std::ios::unitbuf);

    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);

    if (argc < 2){
        printUsage(argv[0]);
        return 1;
    }
 
    std::string algo = "";
    std::string filepath = "";
    std::string directory = "";
    bool testMode = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--algo" && i + 1 < argc) {
            algo = argv[++i];
        } else if (arg == "--file" && i + 1 < argc) {
            filepath = argv[++i];
        } else if (arg == "--dir" && i + 1 < argc) {
            directory = argv[++i];
        } else if (arg == "--test") {
            testMode = true;
        } else if (arg == "--debug") {
            DEBUG_MODE = true;
        } else {
            std::cerr << "Unknown argument: " << arg << "\n";
            printUsage(argv[0]);
            return 1;
        } 
    }

    if (testMode) {
        // if (algo.empty()) {
        //     std::cerr << "Specify an algorithm to test: --algo <dfs|bfs|kruskal|prim> --test\n";
        //     return 1;
        // }
        runTests(directory, algo);
        return 0;
    }

    if (algo.empty()) {
        std::cerr << "No algorithm specified.\n";
        printUsage(argv[0]);
        return 1;
    }

    std::vector<std::string> filesToProcess;

    if (!filepath.empty()) {
        filesToProcess = { filepath };
    } else if (!directory.empty()) {
        filesToProcess = getGraphFiles(directory);
    } else {
        std::cerr << "No --file or --dir specified.\n";
        printUsage(argv[0]);
        return 1;
    }

    for (const auto& f : filesToProcess) {
        try {
            processFile(f, algo);
        } catch (const std::exception& e) {
            std::cerr << "[ERROR] " << f << " — " << e.what() << "\n";
        }
    }

    // std::string filepath = "";
    // void (*traverseFn)(const Graph&, int, std::vector<char>&) = DFS;
    // std::string algoName = "DFS";
    // bool testMode = false;
 
    // for (int i = 1; i < argc; ++i) {
    //     std::string arg = argv[i];
    //     if (arg == "--bfs") {
    //         traverseFn = BFS;
    //         algoName = "BFS";
    //     } else if (arg == "--dfs") {
    //         traverseFn = DFS;
    //         algoName = "DFS";
    //     } else if (arg == "--test") {
    //         testMode = true;
    //     } else {
    //         filepath = arg;
    //     }
    // }
 
    // if (testMode) {
    //     // runTests();
    //     return 0;
    // }
 
    
    // filepath = argv[1];

    // Graph graph = loadGraph(filepath);

    // // int components = mod_components(graph, traverseFn);
    // //std::cout << components;
    
    // double mstWeight = kruskal(graph);

    // std::cout << "Total MST Weight: " << mstWeight << "\n";

    return 0;
}

// std::pair<int, int> parseFilename(const std::string& filepath)
// {
//     std::string filename = std::filesystem::path(filepath).stem().string(); // "G_1_200"
    
//     std::istringstream iss(filename);
//     std::string part;

//     std::getline(iss, part, '_'); // "G"
//     std::getline(iss, part, '_'); // "1"
//     int nodes = std::stoi(part) * 1000;

//     std::getline(iss, part, '_'); // "200"
//     int edges = std::stoi(part) * 1000;

//     return {nodes, edges};
// }

// Graph loadGraph(const std::string& filepath)
// {
//     std::ifstream file(filepath);
//     if (!file) {
//         throw std::runtime_error("Could not open file: " + filepath);
//     }
 
//     int nodeCount, edgeCount = 0;
//     file >> nodeCount;
//     file.ignore();
 
//     if (filepath.find("MST") != std::string::npos) {
//         std::cout << "MST DETECTED";
//         nodeCount = parseFilename(filepath).first;
//         edgeCount = parseFilename(filepath).second;
//     }
    
//     Graph graph(nodeCount, edgeCount);

//     std::cout << "Reading from file..  " << "\n";
//     int v1, v2;
//     double w = 0;

//     auto start = std::chrono::high_resolution_clock::now();

//     // while (file >> v1 >> v2 >> w) {
//     //     graph.addEdge(v1, v2, w);
//     // }

//     std::string line;
//     do {
//         if (line.empty()) continue;

//         std::istringstream iss(line);
//         iss >> v1 >> v2;
//         iss >> w;

//         graph.addEdge(v1, v2, w);
//     } while (std::getline(file, line));
 
//     auto end = std::chrono::high_resolution_clock::now();

//     file.close();
 
//     std::cout << "Graph saved!" << "\n";

//     double ms = std::chrono::duration<double, std::milli>(end - start).count();


//     std::cout << filepath
//               << " | nodes: " << nodeCount
//               << " | reading time: " << ms << " ms\n";
    
//     return graph;
// }