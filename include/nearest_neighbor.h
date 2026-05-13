#include "graph.h"
#include "kruskal.h"
#include "prim.h"

using std::vector, std::pair;

pair<vector<Node*>, double> nearestNeighbor(const Graph& graph);