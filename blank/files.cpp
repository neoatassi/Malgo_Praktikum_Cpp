#include "files.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <filesystem>
#include <chrono>

namespace fs = std::filesystem;

std::vector<std::string> getGraphFiles()
{
    std::vector<std::string> files;
    if (!fs::exists("./Graph") || !fs::is_directory("./Graph")) {
        std::cerr << "Directory ./Graph not found.\n";
        return files;
    }
    for (const auto& entry : fs::directory_iterator("./Graph")) {
        if (entry.is_regular_file()) {
            files.push_back(entry.path().string());
        }
    }
    std::sort(files.begin(), files.end());
    return files;
}

std::vector<std::string> promptFileSelection()
{
    std::vector<std::string> files = getGraphFiles();
    if (files.empty()) {
        std::cerr << "No files found in ./Graph/\n";
        return {};
    }

    std::cout << "No input file specified. Please choose:\n\n";
    for (int i = 0; i < (int)files.size(); ++i) {
        std::cout << "  [" << i + 1 << "] " << files[i] << "\n";
    }
    std::cout << "  [Enter] All files\n\n";
    std::cout << "Choice: ";

    std::string input;
    std::getline(std::cin, input);

    if (input.empty()) {
        return files;
    }

    try {
        int choice = std::stoi(input);
        if (choice < 1 || choice > (int)files.size()) {
            std::cerr << "Invalid choice.\n";
            return {};
        }
        return { files[choice - 1] };
    } catch (...) {
        std::cerr << "Invalid input.\n";
        return {};
    }
}

void processFile(const std::string& filepath, void (Graph::*traverseFn)(int), const std::string& algoName)
{
    std::ifstream file(filepath);
    if (!file) {
        std::cerr << "Could not open file: " << filepath << "\n";
        return;
    }

    int count;
    file >> count;

    Graph graph(count);

    int v1, v2;
    while (file >> v1 >> v2) {
        graph.addEdge(v1, v2);
    }

    file.close();

    auto start = std::chrono::high_resolution_clock::now();
    int result = graph.components(traverseFn);
    auto end = std::chrono::high_resolution_clock::now();

    double ms = std::chrono::duration<double, std::milli>(end - start).count();

    std::cout << "[" << algoName << "] "
              << filepath
              << " | nodes: " << count
              << " | components: " << result
              << " | time: " << ms << " ms\n";
}