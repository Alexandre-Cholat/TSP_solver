#include "parsers.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <zlib.h>

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
    std::string distance_type;


    
    for(const auto& line : read_lines()) {
        if (line.find("DIMENSION") == 0) {
            auto colon_pos = line.find(':');
            dimension = std::stoi(line.substr(colon_pos + 1));
        } else if (line.find("EDGE_WEIGHT_TYPE") == 0) {
            // parse EDGE_WEIGHT_TYPE as distance_type
            auto colon_pos = line.find(':');
            if (colon_pos != std::string::npos) {
                std::string val = line.substr(colon_pos + 1);
                
                // Trim leading and trailing whitespace
                size_t first = val.find_first_not_of(" \t\r\n");
                size_t last = val.find_last_not_of(" \t\r\n");
                
                if (first != std::string::npos && last != std::string::npos) {
                    distance_type = val.substr(first, (last - first + 1));
                }
            }
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

    // define output vector
    std::vector<double> lower_tri_weights;
    lower_tri_weights.reserve((dimension * (dimension - 1)) / 2);

    // Compute array with correct distance calculation
    std::string eucStr ("EUC_2D");
    std::string geoStr ("GEO");
    std::string attStr ("ATT");
    if(distance_type.compare(eucStr) == 0){
        for (int i = 0; i < dimension; ++i) {
            for (int j = 0; j < i; ++j) {
                double dx = nodes[i].x - nodes[j].x;
                double dy = nodes[i].y - nodes[j].y;
                double dist = std::round(std::sqrt(dx * dx + dy * dy));
                lower_tri_weights.push_back(dist);
            }
        }

    }else if(distance_type.compare(geoStr) == 0){
        const double PI = 3.141592;
        const double RRR = 6378.388;

        // calculate lat and long for all nodes
        struct GeoCoord { double lat; double lon; };
        std::vector<GeoCoord> geo_coords;
        geo_coords.reserve(dimension);

        for (const auto& node : nodes) {
            // Calculate Latitude (using x)
            double deg_lat = std::round(node.x);
            double min_lat = node.x - deg_lat;
            double lat = PI * (deg_lat + 5.0 * min_lat / 3.0) / 180.0;

            // Calculate Longitude (using y)
            double deg_lon = std::round(node.y);
            double min_lon = node.y - deg_lon;
            double lon = PI * (deg_lon + 5.0 * min_lon / 3.0) / 180.0;

            geo_coords.push_back({lat, lon});
        }

        // compute GEO distances
        for (int i = 0; i < dimension; ++i) {
            for (int j = 0; j < i; ++j) {
                double q1 = std::cos(geo_coords[i].lon - geo_coords[j].lon);
                double q2 = std::cos(geo_coords[i].lat - geo_coords[j].lat);
                double q3 = std::cos(geo_coords[i].lat + geo_coords[j].lat);

                double cos_val = 0.5 * ((1.0 + q1) * q2 - (1.0 - q1) * q3);

                double dij = static_cast<int>(RRR * std::acos(cos_val) + 1.0);
                lower_tri_weights.push_back(dij);
            }
        }

    }else if(distance_type.compare(attStr) == 0){
        for(int i = 0; i < dimension; ++i){
            for(int j = 0; j < i; ++j) {
                double xd = nodes[i].x - nodes[j].x;
                double yd = nodes[i].y - nodes[j].y;
                double rij = sqrt( (xd*xd + yd*yd) / 10.0 );
                double dij = std::ceil(rij);
                
                lower_tri_weights.push_back(dij);
            }
        }

    }else{
        throw std::runtime_error("Could not identify the following distance_type: " + distance_type);
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