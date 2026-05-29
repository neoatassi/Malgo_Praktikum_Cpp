#include "../include/tests.h"
#include "../include/tsp.h"
#include "../include/files.h"
#include "../include/graph.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <map>
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

// ---------------------------------------------------------------------------
// Small helpers
// ---------------------------------------------------------------------------

namespace {

// Counters for the final summary
int g_passed = 0;
int g_failed = 0;

// Compares two doubles up to the precision the professor reports (2 decimals)
bool approxEqual(double a, double b, double epsilon = 0.01)
{
    return std::abs(a - b) < epsilon;
}

// Reports a single check and updates the counters
void check(bool condition, const std::string& description)
{
    if (condition) {
        std::cout << "  [PASS] " << description << "\n";
        ++g_passed;
    } else {
        std::cout << "  [FAIL] " << description << "\n";
        ++g_failed;
    }
}

// Known optimal tour lengths supplied by the professor.
// Add or edit entries here as more expected values become available.
const std::map<std::string, double> KNOWN_OPTIMA = {
    { "K_10.txt",  38.41 },
    { "K_10e.txt", 27.26 },
    { "K_12.txt",  45.19 },
    { "K_12e.txt", 36.13 },
};

// ---------------------------------------------------------------------------
// Structural validation of a tour, independent of any algorithm.
// A correct TSP tour on n nodes must:
//   1. contain exactly n + 1 entries (start node repeated at the end)
//   2. start and end on the same node
//   3. visit every node exactly once
//   4. have a totalDistance equal to the sum of its edges in the graph
// ---------------------------------------------------------------------------

bool tourIsStructurallyValid(const TourResult& result, const Graph& graph)
{
    int n = graph.getCount();

    if ((int)result.tour.size() != n + 1) return false;
    if (result.tour.front()->getID() != result.tour.back()->getID()) return false;

    std::vector<bool> seen(n, false);
    for (int i = 0; i < n; ++i) {
        int id = result.tour[i]->getID();
        if (id < 0 || id >= n) return false;
        if (seen[id]) return false;
        seen[id] = true;
    }

    return true;
}

// Recomputes the tour length straight from the graph's edge weights,
// so we never trust the number the algorithm reported about itself.
double recomputeTourLength(const TourResult& result, const Graph& graph)
{
    double total = 0.0;
    for (int i = 0; i + 1 < (int)result.tour.size(); ++i) {
        total += graph.getEdgeWeight(result.tour[i]->getID(),
                                     result.tour[i + 1]->getID());
    }
    return total;
}

} // anonymous namespace

// ---------------------------------------------------------------------------
// The actual test routine
// ---------------------------------------------------------------------------

void runTSPTests(const std::string& directory)
{
    std::cout << "\n=== TSP exact-algorithm tests ===\n";
    std::cout << std::fixed << std::setprecision(2);

    int skippedCtr = 0;

    std::vector<std::string> files = getGraphFiles(directory);
    if (files.empty()) {
        std::cout << "No graph files found in " << directory << "\n";
        return;
    }

    for (const std::string& filepath : files) {
        std::string name = fs::path(filepath).filename().string();
        std::cout << "\n" << name << ":\n";

        auto graphPtr = loadGraph(filepath, "UndirectedGraph");
        const Graph& graph = *graphPtr;

        TourResult cs = completeSearch(graph);
        TourResult bb = branchAndBound(graph);

        if (cs.totalDistance == 0 && bb.totalDistance == 0){
            std::cout << "\t[SKIPPED] Graph size is beyond threshold." << "\n";
            skippedCtr++;
            continue;
        };

        // 1. Both tours are structurally valid
        check(tourIsStructurallyValid(cs, graph),
              "complete search returns a valid tour");
        check(tourIsStructurallyValid(bb, graph),
              "branch & bound returns a valid tour");

        // 2. Reported distance matches a fresh recomputation from the graph
        check(approxEqual(cs.totalDistance, recomputeTourLength(cs, graph)),
              "complete search distance matches its tour");
        check(approxEqual(bb.totalDistance, recomputeTourLength(bb, graph)),
              "branch & bound distance matches its tour");

        // 3. The two exact algorithms must agree on the optimal length
        check(approxEqual(cs.totalDistance, bb.totalDistance),
              "complete search and branch & bound agree ("
                  + std::to_string(cs.totalDistance) + " vs "
                  + std::to_string(bb.totalDistance) + ")");

        // 4. Where the professor gave an optimum, both must match it
        auto known = KNOWN_OPTIMA.find(name);
        if (known != KNOWN_OPTIMA.end()) {
            check(approxEqual(cs.totalDistance, known->second),
                  "complete search matches known optimum ("
                      + std::to_string(known->second) + ")");
            check(approxEqual(bb.totalDistance, known->second),
                  "branch & bound matches known optimum ("
                      + std::to_string(known->second) + ")");
        } else {
            std::cout << "  [INFO] no published optimum for this graph; "
                         "checked internal consistency only\n";
        }
    }

    std::cout << "\n=== Summary: " << g_passed << " passed, "
              << g_failed << " failed ===\n\n";
}