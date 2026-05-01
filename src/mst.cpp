#include "../include/mst.h"
#include <numeric>

using std::vector;

struct UnionFind {
    std::vector<int> parent;
    std::vector<int> rank;

    UnionFind(int n) : parent(n), rank(n, 0)
    {
        // each node starts as its own parent
        std::iota(parent.begin(), parent.end(), 0);
    }

    int find(int x)
    {
        // path compression — flattens the tree for future lookups
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    bool unite(int x, int y)
    {
        int rootX = find(x);
        int rootY = find(y);

        if (rootX == rootY) return false; // already in same component

        // union by rank — attach smaller tree under larger tree
        if (rank[rootX] < rank[rootY]) std::swap(rootX, rootY);
        parent[rootY] = rootX;
        if (rank[rootX] == rank[rootY]) rank[rootX]++;

        return true;
    }
};

double kruskal (const Graph& graph){
    
    auto start = std::chrono::high_resolution_clock::now();

    double mst = 0;
    int addedEdges = 0;

    vector<Weighted> edges = graph.getEdges();

    std::sort(edges.begin(), edges.end(),
        [](const auto& e1, const auto& e2) {
        return e1.weight < e2.weight;
    });

    UnionFind uf(graph.getCount());

    for (const Weighted& edge: edges){
        if (uf.unite(edge.src, edge.dest)) {
            mst += edge.weight;
            addedEdges++;

            if (addedEdges == graph.getCount() - 1)
                break;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    double ms = std::chrono::duration<double, std::milli>(end - start).count();

    std::cout << "Processed MST in " << ms << "ms" << "\n"; 

    return mst;
}