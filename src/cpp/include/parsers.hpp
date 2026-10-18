#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <zlib.h>

class BaseParser {
protected:
    std::string filepath;
    int dimension = 0;

public:
    BaseParser(const std::string& path) : filepath(path) {}
    virtual ~BaseParser() = default;

    // Returns a pair: {dimension, flattened_lower_triangular_matrix}
    virtual std::pair<int, std::vector<double>> parse() = 0;

    std::vector<std::string> read_lines() {
        std::vector<std::string> lines;
        
        // Transparently handle .gz files
        if (filepath.size() >= 3 && filepath.substr(filepath.size() - 3) == ".gz") {
            gzFile file = gzopen(filepath.c_str(), "r");
            if (!file) throw std::runtime_error("Could not open .gz file");
            
            char buffer[4096];
            while (gzgets(file, buffer, sizeof(buffer))) {
                std::string line(buffer);
                line.erase(line.find_last_not_of(" \n\r\t") + 1); // trim trailing whitespace
                if (!line.empty()) lines.push_back(line);
            }
            gzclose(file);
        } else {
            // Standard plain text reading
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
};

struct Node {
    int id;
    double x, y;
};

class CoordParser : public BaseParser {
public:
    CoordParser(const std::string& path) : BaseParser(path) {}

    std::pair<int, std::vector<double>> parse() override {
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
                    nodes.push_back({id - 1, x, y}); // 0-indexed internally
                }
            }
        }

        // Sort by ID to ensure proper ordering
        std::sort(nodes.begin(), nodes.end(), [](const Node& a, const Node& b) {
            return a.id < b.id;
        });

        // Compute lower triangular distance array (Euclidean, rounded to int)[cite: 2]
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
};

class LowerTriParser : public BaseParser {
public:
    LowerTriParser(const std::string& path) : BaseParser(path) {}

    std::pair<int, std::vector<double>> parse() override {
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
                        // Skip non-numeric entries as done in the Python logic
                        continue;
                    }
                }
            }
        }
        return {dimension, matrix_data};
    }
};