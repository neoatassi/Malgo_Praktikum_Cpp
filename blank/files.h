#pragma once

#include <string>
#include <vector>
#include "graph.h"

std::vector<std::string> getGraphFiles();
std::vector<std::string> promptFileSelection();
void processFile(const std::string& filepath, void (Graph::*traverseFn)(int), const std::string& algoName);