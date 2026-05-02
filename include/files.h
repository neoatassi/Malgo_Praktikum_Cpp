#pragma once

#include <string>
#include <vector>
#include "../include/graph.h"
#include "../include/debug.h"

// std::vector<std::string> getGraphFiles();
// std::vector<std::string> promptFileSelection();
// void processFile(const std::string& filepath, void (Graph::*traverseFn)(int), const std::string& algoName);

std::vector<std::string> getGraphFiles(const std::string& directory);
std::pair<int, int> parseFilename(const std::string& filepath);
bool isValidGraphFilename(const std::string& filepath);
Graph loadGraph(const std::string& filepath);