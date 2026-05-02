#pragma once

#include <vector>
#include <stack>
#include "../include/graph.h"
#include "../include/debug.h"


void DFS (const Graph& graph, int vertex, std::vector<char>& visited);
void BFS(const Graph& graph, int vertex, std::vector<char>& visited);

int countComponents(const Graph& graph, void (*traverseFn)(const Graph&, int, std::vector<char>&));
