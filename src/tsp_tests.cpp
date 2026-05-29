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
#include <sstream>

namespace fs = std::filesystem;

// ---------------------------------------------------------------------------
// Small helpers
// ---------------------------------------------------------------------------

namespace {

// Counters for the final summary
int g_passed = 0;
int g_failed = 0;

// Helper to prevent trailing zeroes when outputting to_string(double)
std::string roundDoubleToString(double value, int precision) {
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(precision) << value;
    return stream.str();
}

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
// Structural validation check of the tour independent of algorithm.
// ---------------------------------------------------------------------------

bool tourIsStructurallyValid(const TourResult& result, const Graph& graph)
{
    int n = graph.getCount();

    // Must contain exactly n + 1 entries (start node repeated at the end)
    if ((int)result.tour.size() != n + 1) return false;

    // Must start and end on the same node
    if (result.tour.front()->getID() != result.tour.back()->getID()) return false;

    // must visit every node exactly once
    std::vector<bool> seen(n, false);
    for (int i = 0; i < n; ++i) {
        int id = result.tour[i]->getID();
        if (id < 0 || id >= n) return false;
        if (seen[id]) return false;
        seen[id] = true;
    }

    return true;
}

// Recomputes the tour length straight from the graph's edge weights.
// should have a totalDistance equal to the sum of its edges in the graph.
double recomputeTourLength(const TourResult& result, const Graph& graph)
{
    double total = 0.0;
    for (int i = 0; i + 1 < (int)result.tour.size(); ++i) {
        total += graph.getEdgeWeight(result.tour[i]->getID(),
                                     result.tour[i + 1]->getID());
    }
    return total;
}

}

// ---------------------------------------------------------------------------
// Test routine
// ---------------------------------------------------------------------------

void runTSPTests(const std::string& directory)
{
    std::cout << "\n=== TSP exact-algorithm tests ===\n";
    std::cout << std::fixed << std::setprecision(2);

    int skippedCtr = 0;

    int precision = 2;

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
            std::cout << "[SKIPPED] Graph size is beyond threshold." << "\n";
            skippedCtr++;
            continue;
        };

        // Check tour structural validity
        check(tourIsStructurallyValid(cs, graph),
              "complete search returns a valid tour");
        check(tourIsStructurallyValid(bb, graph),
              "branch & bound returns a valid tour");

        // Distance matches a fresh recomputation from the graph
        check(approxEqual(cs.totalDistance, recomputeTourLength(cs, graph)),
              "complete search distance matches its tour");
        check(approxEqual(bb.totalDistance, recomputeTourLength(bb, graph)),
              "branch & bound distance matches its tour");

        // Both completeSearch and Branch&Bound must deliver same totalDistance
        check(approxEqual(cs.totalDistance, bb.totalDistance),
              "complete search and branch & bound agree ("
                  + roundDoubleToString(cs.totalDistance, precision) + " vs "
                  + roundDoubleToString(bb.totalDistance, precision) + ")");

        // Compare with known optima
        auto known = KNOWN_OPTIMA.find(name);
        if (known != KNOWN_OPTIMA.end()) {
            check(approxEqual(cs.totalDistance, known->second),
                  "complete search matches known optimum ("
                      + roundDoubleToString(known->second, precision) + ")");
            check(approxEqual(bb.totalDistance, known->second),
                  "branch & bound matches known optimum ("
                      + roundDoubleToString(known->second, precision) + ")");
        } else {
            std::cout << "  [INFO] no published optimum for this graph; "
                         "checked internal consistency only\n";
        }
    }

    std::cout << "\n=== Summary: " << g_passed << " passed, "
              << g_failed << " failed ===\n\n";
}