#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "LowerTriMatrix.hpp"
#include "algorithms/trivial.hpp"

// Lightweight JSON parsing for simple {"dimension": N, "weights": [...]}
bool load_parsed_data(const std::string& filepath, size_t& dim, std::vector<double>& weights) {
    std::ifstream file(filepath);
    if (!file.is_open()) return false;

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    
    // Extract dimension
    size_t dim_pos = content.find("\"dimension\":");
    if (dim_pos == std::string::npos) return false;
    dim = std::stoul(content.substr(dim_pos + 12));

    // Extract weights array
    size_t weights_pos = content.find("\"weights\":");
    if (weights_pos == std::string::npos) return false;

    size_t start_arr = content.find('[', weights_pos);
    size_t end_arr = content.find(']', start_arr);
    if (start_arr == std::string::npos || end_arr == std::string::npos) return false;

    std::string array_str = content.substr(start_arr + 1, end_arr - start_arr - 1);
    size_t pos = 0;
    while (pos < array_str.length()) {
        size_t next_comma = array_str.find(',', pos);
        if (next_comma == std::string::npos) next_comma = array_str.length();
        
        std::string val_str = array_str.substr(pos, next_comma - pos);
        if (!val_str.empty()) {
            weights.push_back(std::stod(val_str));
        }
        pos = next_comma + 1;
    }

    return dim > 0 && !weights.empty();
}

void write_tour_file(const std::string& output_path, const std::string& instance_name, const std::vector<int>& tour) {
    std::ofstream out(output_path);
    out << "NAME : " << instance_name << ".tour\n";
    out << "TYPE : TOUR\n";
    out << "DIMENSION : " << tour.size() << "\n";
    out << "TOUR_SECTION\n";
    for (int node_id : tour) {
        out << node_id << "\n";
    }
    out << "-1\n";
    out << "EOF\n";
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: ./build/TSPsolver <parsed_json_path> <output_path> [-m algorithm]\n";
        return 1;
    }

    std::string json_path = argv[1];
    std::string output_path = argv[2];
    std::string method = "trivial";

    for (int i = 3; i < argc; ++i) {
        if (std::string(argv[i]) == "-m" && i + 1 < argc) {
            method = argv[i + 1];
        }
    }

    size_t dim = 0;
    std::vector<double> weights;

    if (!load_parsed_data(json_path, dim, weights)) {
        std::cerr << "Error: Could not read parsed data from " << json_path << std::endl;
        return 1;
    }

    LowerTriMatrix matrix(dim, weights);

    if (method == "trivial") {
        std::vector<int> tour = solve_trivial(matrix.get_dimension());
        double length = calculate_tour_length(matrix, tour);
        
        std::cout << "Trivial Method Completed. Tour Length: " << length << std::endl;
        write_tour_file(output_path, "solution", tour);
        std::cout << "Tour written to: " << output_path << std::endl;
    }

    return 0;
}