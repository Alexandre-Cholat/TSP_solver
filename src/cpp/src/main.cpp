#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include "parsers.hpp"
#include "Solver.hpp"
#include "TrivialSolver.hpp"

// Usage: ./TSPsolver </home/alexandre/Documents/ai_m1/Programming/TSP_solver/ali535.tsp.gz> </home/alexandre/Documents/ai_m1/Programming/TSP_solver/ali535.tsp.tour> -m trivial
// ./TSPsolver /home/alexandre/Documents/ai_m1/Programming/TSP_solver/ali535.tsp.gz /home/alexandre/Documents/ai_m1/Programming/TSP_solver/ali535.tsp.tour -m trivial
// Factory function to peek at the file and return the appropriate parser
std::unique_ptr<BaseParser> get_parser(const std::string& filepath) {
    // Instantiate a temporary parser just to use the read_lines() utility
    std::unique_ptr<BaseParser> peeker = std::make_unique<LowerTriParser>(filepath);
    
    for (const auto& line : peeker->read_lines()) {
        if (line.find("NODE_COORD_SECTION") != std::string::npos) {
            return std::make_unique<CoordParser>(filepath);
        }
        if (line.find("EDGE_WEIGHT_SECTION") != std::string::npos) {
            return std::make_unique<LowerTriParser>(filepath);
        }
    }
    throw std::runtime_error("Could not determine format for " + filepath);
}

int main(int argc, char* argv[]) {
    // 1. Parse command line arguments[cite: 2]
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <instance_path> <output_path> [-m <algorithm>]\n";
        return 1;
    }

    std::string instance_path = argv[1];
    std::string output_path = argv[2];
    std::string method = "trivial"; // default[cite: 2]

    for (int i = 3; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-m" && i + 1 < argc) {
            method = argv[i + 1];
        }
    }

    // 2. Parse the input file
    auto parser = get_parser(instance_path);
    auto data = parser->parse();
    int dimension = data.first;
    std::vector<double>& weights = data.second;

    // 3. Select and run the algorithm
    std::unique_ptr<Solver> solver;
    if (method == "trivial") {
        solver = std::make_unique<TrivialSolver>();
    } else {
        std::cerr << "Error: Unknown or unimplemented method '" << method << "'\n";
        return 1;
    }

    std::vector<int> tour = solver->solve(weights.data(), dimension);

    // 4. Write output strictly matching the required TOUR_SECTION format[cite: 3]
    std::ofstream out_f(output_path);
    if (!out_f.is_open()) {
        std::cerr << "Error: Could not open output file.\n";
        return 1;
    }

    // Extract filename without extension for the NAME field
    size_t last_slash = instance_path.find_last_of("/\\");
    std::string filename = (last_slash == std::string::npos) ? instance_path : instance_path.substr(last_slash + 1);
    size_t last_dot = filename.find_last_of('.');
    if (last_dot != std::string::npos) filename = filename.substr(0, last_dot);

    out_f << "NAME: " << filename << ".tour\n";
    out_f << "TYPE: TOUR\n";
    out_f << "DIMENSION: " << dimension << "\n";
    out_f << "TOUR_SECTION\n";
    
    for (int node : tour) {
        out_f << node << "\n";
    }
    
    out_f << "-1\n";
    out_f << "EOF\n";
    out_f.close();

    return 0;
}