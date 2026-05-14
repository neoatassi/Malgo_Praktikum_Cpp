#include "../include/kruskal.h"
#include <memory>
//#include <numeric>

using std::vector;

struct UnionFind {
    std::vector<int> parent;
    std::vector<int> rank;

    UnionFind(int n)
    {
        parent.resize(n);
        rank.resize(n, 0);

        // for every element: index = value -> every node is its own parent
        for (int i = 0; i < n; ++i) {
            parent[i] = i;
        }
    }

    int find(int x)
    {
        // path compression — flattens the tree for future lookups
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    bool unite(int u, int v)
    {
        if (u == v) return false;
        
        // union by rank — attach smaller tree under larger tree
        if (rank[u] < rank[v]) std::swap(u, v);
        parent[v] = u;
        if (rank[u] == rank[v]) rank[u]++;

        return true;
    }
};

double kruskal (const Graph& graph){

    double mst = 0;
    int addedEdges = 0;

    vector<Edge> sortedEdges = graph.getEdges();

    // for (const auto& edgePtr : graph.getEdges()) {
    //     sortedEdges.push_back(edgePtr.get());
    // }

    debugLog("sortedEdges size PRE sorting" + std::to_string(sortedEdges.size()) + "\n");

    std::sort(sortedEdges.begin(), sortedEdges.end(),
        [](const Edge& e1, const Edge& e2) {
        return e1.weight < e2.weight;
    });

    debugLog("sortedEdges size POST sorting" + std::to_string(sortedEdges.size()) + "\n");

    // make a set for every node
    UnionFind uf(graph.getCount());

    // for (Edge* edge: sortedEdges){

    //     int u = uf.find(edge->src->getID());
    //     int v = uf.find(edge->dest->getID());

    //     if (uf.unite(u, v)) {
    //         mst += edge->weight;
    //         addedEdges++;

    //         // stops after reaching e = n - 1, otherwise creates cycles
    //         if (addedEdges == graph.getCount() - 1)
    //             break;
    //     }
    // }

    for (const Edge& edge : sortedEdges){

        int u = uf.find(edge.src);
        int v = uf.find(edge.dest);

        if (uf.unite(u, v)){
            mst += edge.weight;
            addedEdges++;

            if (addedEdges == graph.getCount() - 1){
                debugLog("Stopped at: " + std::to_string(addedEdges) + " edges, nodeCount: " + std::to_string(graph.getCount()) + "\n");
                break;
            }
        }
    }


    return mst;
}

// double kruskal (const Graph& graph){
    
    // auto start = std::chrono::high_resolution_clock::now();

    // double mst = 0;
    // int addedEdges = 0;

    // vector<Weighted> edges = graph.getEdges();

    // std::sort(edges.begin(), edges.end(),
        // [](const auto& e1, const auto& e2) {
        // return e1.weight < e2.weight;
    // });

    // // make a set for every node
    // UnionFind uf(graph.getCount());

    // for (const Weighted& edge: edges){

        // int u = uf.find(edge.src);
        // int v = uf.find(edge.dest);

        // if (u != v) {
            // uf.unite(u, v);
            // mst += edge.weight;
            // addedEdges++;

            // // stops after reaching e = n - 1, otherwise creates cycles
            // if (addedEdges == graph.getCount() - 1)
                // break;
        // }
    // }

    // auto end = std::chrono::high_resolution_clock::now();
    // double ms = std::chrono::duration<double, std::milli>(end - start).count();

    // debugLog("Processed MST in " + std::to_string(ms) + "ms");
    // // std::cout << "Processed MST in " << ms << "ms" << "\n"; 

    // return mst;
// }