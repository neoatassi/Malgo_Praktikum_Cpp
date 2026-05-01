#pragma once

#include <vector>
#include <stack>
#include "../include/graph.h"


void DFS (const Graph& graph, int vertex, std::vector<char>& visited);
void BFS(const Graph& graph, int vertex, std::vector<char>& visited);

int mod_components(const Graph& graph, void (*traverseFn)(const Graph&, int, std::vector<char>&));
