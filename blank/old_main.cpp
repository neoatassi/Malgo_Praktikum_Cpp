// void processFile(const std::string& filepath, void (Graph::*traverseFn)(int), const std::string& algoName)
// {
//     std::ifstream file(filepath);
//     if (!file) {
//         std::cerr << "Could not open file: " << filepath << "\n";
//         return;
//     }
 
//     int count;
//     file >> count;
 
//     Graph graph(count);
 
//     int v1, v2;
//     while (file >> v1 >> v2) {
//         graph.addEdge(v1, v2);
//     }
 
//     file.close();
 
//     // measure time
//     auto start = std::chrono::high_resolution_clock::now();
//     int result = graph.mod_components(traverseFn);
//     auto end = std::chrono::high_resolution_clock::now();
 
//     double ms = std::chrono::duration<double, std::milli>(end - start).count();
 
//     std::cout << "[" << algoName << "] "
//               << filepath
//               << " | nodes: " << count
//               << " | components: " << result
//               << " | time: " << ms << " ms\n";
// }

// int main(int argc, char* argv[])
// {
//     if (argc < 2) {
//         std::cerr << "Usage: " << argv[0] << " <input file>" << "\n";
//         return 1;
//     }

//     std::string filepath = argv[1];

//     std::ifstream file;
//     file.open(filepath, std::ios::in);

//     if (file) {
//         std::cout << "Good!" << "\n";
        
//         int count;
//         file>>count;

//         Graph graph(count);

//         std::cout << "Knotenanzahl: " << count << "\n";
        
//         int v1, v2;
//         //std::vector<std::vector<int>> vertices (count);

//         while (file>>v1>>v2){
            
//             //vertices[v1][v2] = 1;

//             graph.addEdge(v1, v2);

//             // if(file.eof())
//             //     break;
//         }

//         //graph.printGraph();

//         //graph.DFS(0, graph.getVisited());

//         runTests();

//         std::cout << "Component count: " << graph.components() << "\n";

//         file.close();

//     } else {
//         std::cout << "Not good";
//         return 1;
//     }

// }