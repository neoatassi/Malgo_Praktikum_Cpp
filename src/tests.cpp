#include "tests.h"
#include "graph.h"
void assertEqual(int expected, int actual, const std::string& testName)
{
    if (expected == actual) {
        std::cout << "[PASS] " << testName << "\n";
    } else {
        std::cout << "[FAIL] " << testName
                  << " | expected: " << expected
                  << ", got: " << actual << "\n";
    }
}
 
void runTests()
{
    std::cout << "\n--- Running unit tests ---\n";
 
    // Test 1: single node, no edges -> 1 component
    {
        Graph g(1);
        assertEqual(1, g.mod_components(&Graph::DFS), "DFS | single node");
        g.reset();
        assertEqual(1, g.mod_components(&Graph::BFS), "BFS | single node");
    }
 
    // Test 2: fully connected graph -> 1 component
    {
        Graph g(4);
        g.addEdge(0, 1);
        g.addEdge(1, 2);
        g.addEdge(2, 3);
        assertEqual(1, g.mod_components(&Graph::DFS), "DFS | fully connected (chain)");
        g.reset();
        assertEqual(1, g.mod_components(&Graph::BFS), "BFS | fully connected (chain)");
    }
 
    // Test 3: no edges -> every node is its own component
    {
        Graph g(5);
        assertEqual(5, g.mod_components(&Graph::DFS), "DFS | no edges, 5 isolated nodes");
        g.reset();
        assertEqual(5, g.mod_components(&Graph::BFS), "BFS | no edges, 5 isolated nodes");
    }
 
    // Test 4: two clear mod_components
    {
        Graph g(6);
        g.addEdge(0, 1);
        g.addEdge(1, 2);
        g.addEdge(3, 4);
        g.addEdge(4, 5);
        assertEqual(2, g.mod_components(&Graph::DFS), "DFS | two mod_components");
        g.reset();
        assertEqual(2, g.mod_components(&Graph::BFS), "BFS | two mod_components");
    }
 
    // Test 5: three mod_components, one isolated node
    {
        Graph g(7);
        g.addEdge(0, 1);
        g.addEdge(2, 3);
        g.addEdge(3, 4);
        // node 5 and 6 are isolated
        assertEqual(4, g.mod_components(&Graph::DFS), "DFS | three mod_components + two isolated nodes");
        g.reset();
        assertEqual(4, g.mod_components(&Graph::BFS), "BFS | three mod_components + two isolated nodes");
    }
 
    std::cout << "--- Tests done ---\n\n";
}