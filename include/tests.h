
#include "../include/files.h"
#include "../include/kruskal.h"
#include "../include/prim.h"
#include "../include/components.h"
#include "../include/tsp.h"

double roundTo(double value, int decimals);
static void runComponentTests(const std::string& directory, void (*traverseFn)(const Graph&, int, std::vector<char>&));
void runMSTTests(const std::string& directory, MSTResult (*mstFn)(const Graph&), const std::string& algoName);
void runTests(const std::string& directory, const std::string& algo);
bool approximatelyEqual(double a, double b, double epsilon);
