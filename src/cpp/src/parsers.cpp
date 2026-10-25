#include "parsers.hpp"

#include <cstddef>
#include <vector>


std::vector<std::string> BaseParser:: read_lines(){
    std::vector<std::string> lines;
        
    //  .gz file format
    if (filepath.size() >= 3 && filepath.substr(filepath.size() - 3) == ".gz") {
        gzFile file = gzopen(filepath.c_str(), "r");
        if (!file) throw std::runtime_error("Could not open .gz file");
        
        char buffer[4096];
        while (gzgets(file, buffer, sizeof(buffer))) {
            std::string line(buffer);
            line.erase(line.find_last_not_of(" \n\r\t") + 1); // trim whitespace
            if (!line.empty()) lines.push_back(line);
        }
        gzclose(file);
    } else {
        // txt file format
        std::ifstream file(filepath);
        if (!file.is_open()) throw std::runtime_error("Could not open file: " + filepath);
        
        std::string line;
        while (std::getline(file, line)) {
            line.erase(line.find_last_not_of(" \n\r\t") + 1);
            if (!line.empty()) lines.push_back(line);
        }
    }
    return lines;
}

std::pair<int, std::vector<double>> CoordParser:: parse() {
    bool reading_coords = false;
    std::vector<Node> nodes;

    for (const auto& line : read_lines()) {
        if (line.find("DIMENSION") == 0) {
            auto colon_pos = line.find(':');
            dimension = std::stoi(line.substr(colon_pos + 1));
        } else if (line.find("NODE_COORD_SECTION") == 0) {
            reading_coords = true;
        } else if (reading_coords) {
            if (line.find("EOF") == 0) break;
            
            std::stringstream ss(line);
            int id;
            double x, y;
            if (ss >> id >> x >> y) {
                nodes.push_back({id - 1, x, y});
            }
        }
    }

    // Sort by ID
    std::sort(nodes.begin(), nodes.end(), [](const Node& a, const Node& b) {
        return a.id < b.id;
    });

    // TODO : support other distance types (ATT, GEO, etc.)
    // Compute distance array (Euclidean, rounded to int)
    std::vector<double> lower_tri_weights;
    lower_tri_weights.reserve((dimension * (dimension - 1)) / 2);

    for (int i = 0; i < dimension; ++i) {
        for (int j = 0; j < i; ++j) {
            double dx = nodes[i].x - nodes[j].x;
            double dy = nodes[i].y - nodes[j].y;
            double dist = std::round(std::sqrt(dx * dx + dy * dy));
            lower_tri_weights.push_back(dist);
        }
    }

    return {dimension, lower_tri_weights};


}

std::pair<int, std::vector<double>> LowerTriParser:: parse(){
    bool reading_matrix = false;
    std::vector<double> matrix_data;

    for (const auto& line : read_lines()) {
        if (line.find("DIMENSION") == 0) {
            auto colon_pos = line.find(':');
            dimension = std::stoi(line.substr(colon_pos + 1));
        } else if (line.find("EDGE_WEIGHT_SECTION") == 0) {
            reading_matrix = true;
        } else if (reading_matrix) {
            if (line.find("EOF") == 0) break;
            
            std::stringstream ss(line);
            std::string val_str;
            while (ss >> val_str) {
                try {
                    matrix_data.push_back(std::stod(val_str));
                } catch (...) {
                    // Skip not number vals
                    continue;
                }
            }
        }
    }
    return {dimension, matrix_data};
}