

#include <cmath>
#include <filesystem>
#include <map>
#include <memory>
#include <string>
#include "../include/tests.h"


// --- Component tests (DFS / BFS) ---

struct ComponentExpected {
    int components;
};

const std::map<std::string, ComponentExpected> componentExpected = {
    { "Graph1.txt", { 2 } },
    { "Graph2.txt", { 4 } },
    { "Graph3.txt", { 4 } },
    { "Graph_gross.txt", { 222 } },
    { "Graph_ganzgross.txt", { 9560 } },
    { "Graph_ganzganzgross.txt", { 306 } },
    // fill in from your expected values
};

static void runComponentTests(const std::string& directory, void (*traverseFn)(const Graph&, int, std::vector<char>&))
{
    std::string algoName = (traverseFn == &DFS) ? "DFS" : "BFS";
    std::cout << "\n--- Running tests for: " << algoName << " ---\n\n";

    std::vector<std::string> files = getGraphFiles(directory);

    int passed = 0;
    int failed = 0;

    for (const auto& filepath : files) {
        std::string filename = std::filesystem::path(filepath).filename().string();

        if (componentExpected.find(filename) == componentExpected.end()) {
            std::cout << "[SKIP] " << filename << " — no expected result\n";
            continue;
        }

        try {
            std::unique_ptr<Graph> graph = loadGraph(filepath, "UndirectedGraph");

            auto start = std::chrono::high_resolution_clock::now();
            int result = countComponents(*graph, traverseFn);
            auto end = std::chrono::high_resolution_clock::now();
            double ms = std::chrono::duration<double, std::milli>(end - start).count();

            int expected = componentExpected.at(filename).components;
            if (result == expected){
                std::cout << "[PASS] " << filename << "\t| components: " << result << "\t";
                passed++;
            } else {
                std::cout << "[FAIL] " << filename
                          << "\t| expected result: " << expected
                          << ", got: " << result;
                failed++;
            }

            std::cout << "\ttime: " << ms << " ms\n";

        } catch (const std::exception& e) {
            std::cout << "[ERROR] " << filename << " — " << e.what() << "\n";
        }
    }

    std::cout << "\n--- Results: "
              << passed << " passed, "
              << failed << " failed ---\n";

}

// --------- MST TESTS -------------------

// used to determining tolerance margin for elapsed runtime (percentage)
const double tolerance = 10.0;

struct ExpectedResult {
    double mstWeight;        // expected total MST weight
    double maxTimeMs;     // expected maximum runtime in ms
};

std::map<std::string, ExpectedResult> expectedResultsKruskal = {
    { "G_1_2.txt",    { 287.32286, 3  } },
    { "G_1_20.txt",   { 36.86275, 10 } },
    { "G_1_200.txt",  { 12.68182, 120 } },
    { "G_10_20.txt",  { 2785.62417, 20 } },
    { "G_10_200.txt",  { 372.14417, 110 } },
    { "G_100_200.txt",  { 27550.51488, 240 } },
};

std::map<std::string, ExpectedResult> expectedResultsPrim = {
    { "G_1_2.txt",    { 287.32286, 3  } },
    { "G_1_20.txt",   { 36.86275, 10 } },
    { "G_1_200.txt",  { 12.68182, 120 } },
    { "G_10_20.txt",  { 2785.62417, 20 } },
    { "G_10_200.txt",  { 372.14417, 110 } },
    { "G_100_200.txt",  { 27550.51488, 240 } },
};

double roundTo(double value, int decimals)
{
    double factor = std::pow(10.0, decimals);
    return std::round(value * factor) / factor;
}

bool approximatelyEqual(double a, double b, double epsilon = 1e-6)
{
    return std::abs(a - b) < epsilon;
}

void runMSTTests(const std::string& directory, double (*mstFn)(const Graph&), const std::string& algoName)
{
    std::cout << "\n--- Running tests for: " << algoName << " ---\n\n";

    std::vector<std::string> files = getGraphFiles(directory);

    int passed = 0;
    int failed = 0;
    int warned = 0; // used for slow processing times

    for (const std::string& filepath : files) {
        std::string filename = std::filesystem::path(filepath).filename().string();

        // skip files not in expected results
        if (expectedResultsKruskal.find(filename) == expectedResultsKruskal.end()) {
            std::cout << "[SKIP] " << filename << " — no expected result defined\n";
            continue;
        }

        // Evaluate which values to compare to depending on algorithm
        ExpectedResult expected;
        if (algoName == "kruskal")
            expected = expectedResultsKruskal[filename];
        else expected = expectedResultsPrim[filename];

        try {
            std::unique_ptr<Graph> graph = loadGraph(filepath, "UndirectedGraph");

            auto start = std::chrono::high_resolution_clock::now();
            double result = mstFn(*graph);
            auto end = std::chrono::high_resolution_clock::now();
            double ms = std::chrono::duration<double, std::milli>(end - start).count();

            // double roundedResult = roundTo(result, 5);
            // double roundedExpected = roundTo(expected.mstWeight, 5);
            
            // check result
            if (approximatelyEqual(result, expected.mstWeight)) {
                std::cout << "[PASS] " << filename << "\t| weight: " << result << "\t";
                passed++;
            } else {
                std::cout << "[FAIL] " << filename
                          << "\t| expected weight: " << expected.mstWeight
                          << ", got: " << result << "\n";
                failed++;
            }

            double toleranceMs = expected.maxTimeMs * (1.0 + tolerance / 100.0);
            // check runtime
            std::cout << "time: " << ms << " ms"
                      << "\t| goal: " << expected.maxTimeMs << " ms"
                      << "\t| tolerance ceiling: " << toleranceMs << " ms";

            if (ms > toleranceMs) {
                std::cout << "  [SLOW]\n";
                warned++;
            } else {
                std::cout << "  [OK]\n";
            }

        } catch (const std::exception& e) {
            std::cout << "[ERROR] " << filename << " — " << e.what() << "\n";
            failed++;
        }
    }

    std::cout << "\n--- Results: "
              << passed << " passed, "
              << failed << " failed, "
              << warned  << " slow ---\n";
}

void runTests(const std::string& directory, const std::string& algo)
{

    if (algo == "dfs") {
        runComponentTests(directory, &DFS);
    } else if (algo == "bfs") {
        runComponentTests(directory, &BFS);
    } else if (algo == "kruskal") {
        runMSTTests(directory, &kruskal, "Kruskal");
    } else if (algo == "prim") {
        runMSTTests(directory, &prim, "Prim");
    } else {
        std::cout << "\n--- No algorithm specified - Running on all" << " ---\n\n";
        runComponentTests("./Graph/Components", &DFS);
        runComponentTests("./Graph/Components", &DFS);
        runMSTTests("./Graph/MST", &kruskal, "Kruskal");
        runMSTTests("./Graph/MST", &prim, "Prim");
        //std::cerr << "No tests defined for algorithm: " << algo << "\n";
    }

    std::cout << "\n--- Done ---\n\n";
}