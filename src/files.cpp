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
    std::sort(files.begin(), files.end());
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
        debugLog("MST FILENAME CONVENTION DETECTED");
        nodeCount = parseFilename(filepath).first;
        edgeCount = parseFilename(filepath).second;
    }
    
    Graph graph(nodeCount, edgeCount);

    //std::cout << "Reading from file..  " << "\n";
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
 
    //std::cout << "Graph saved!" << "\n";

    double ms = std::chrono::duration<double, std::milli>(end - start).count();


    // std::cout << filepath
    //           << " | nodes: " << nodeCount
    //           << " | reading time: " << ms << " ms\n";
    
    return graph;
}

// void processFile(const std::string& filepath, void (Graph::*traverseFn)(int), const std::string& algoName)
// {
//     std::ifstream file(filepath);
//     if (!file) {
//         std::cerr << "Could not open file: " << filepath << "\n";
//         return;
//     }

//     int count;
//     file >> count;

//     Graph graph(count);

//     int v1, v2;
//     while (file >> v1 >> v2) {
//         graph.addEdge(v1, v2);
//     }

//     file.close();

//     auto start = std::chrono::high_resolution_clock::now();
//     int result = graph.components(traverseFn);
//     auto end = std::chrono::high_resolution_clock::now();

//     double ms = std::chrono::duration<double, std::milli>(end - start).count();

//     std::cout << "[" << algoName << "] "
//               << filepath
//               << " | nodes: " << count
//               << " | components: " << result
//               << " | time: " << ms << " ms\n";
// }

// std::vector<std::string> promptFileSelection()
// {
//     std::vector<std::string> files = getGraphFiles();
//     if (files.empty()) {
//         std::cerr << "No files found in ./Graph/\n";
//         return {};
//     }

//     std::cout << "No input file specified. Please choose:\n\n";
//     for (int i = 0; i < (int)files.size(); ++i) {
//         std::cout << "  [" << i + 1 << "] " << files[i] << "\n";
//     }
//     std::cout << "  [Enter] All files\n\n";
//     std::cout << "Choice: ";

//     std::string input;
//     std::getline(std::cin, input);

//     if (input.empty()) {
//         return files;
//     }

//     try {
//         int choice = std::stoi(input);
//         if (choice < 1 || choice > (int)files.size()) {
//             std::cerr << "Invalid choice.\n";
//             return {};
//         }
//         return { files[choice - 1] };
//     } catch (...) {
//         std::cerr << "Invalid input.\n";
//         return {};
//     }
// }

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

// static std::string promptSubdirectory()
// {
//     std::cout << "\nSelect graph directory:\n\n";
//     for (int i = 0; i < (int)SUBDIRS.size(); ++i) {
//         std::cout << "  [" << i + 1 << "] " << SUBDIRS[i] << "\n";
//     }
//     std::cout << "\nChoice: ";

//     std::string input;
//     std::getline(std::cin, input);

//     try {
//         int choice = std::stoi(input);
//         if (choice < 1 || choice > (int)SUBDIRS.size()) {
//             std::cerr << "Invalid choice.\n";
//             return "";
//         }
//         return BASE_DIR + "/" + SUBDIRS[choice - 1];
//     } catch (...) {
//         std::cerr << "Invalid input.\n";
//         return "";
//     }
// }