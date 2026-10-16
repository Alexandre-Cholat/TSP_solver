#ifndef TRIVIAL_HPP
#define TRIVIAL_HPP

#include <vector>
#include <cstddef>
#include "../LowerTriMatrix.hpp"

// Calculates total length of tour: 1 -> 2 -> ... -> N -> 1
inline double calculate_tour_length(const LowerTriMatrix& matrix, const std::vector<int>& tour) {
    double total_length = 0.0;
    size_t n = tour.size();

    for (size_t i = 0; i < n - 1; ++i) {
        total_length += matrix.get_distance(tour[i] - 1, tour[i + 1] - 1);
    }
    // Return to start node
    total_length += matrix.get_distance(tour[n - 1] - 1, tour[0] - 1);

    return total_length;
}

// Generates 1-indexed sequential tour [1, 2, 3, ..., N]
inline std::vector<int> solve_trivial(size_t dimension) {
    std::vector<int> tour(dimension);
    for (size_t i = 0; i < dimension; ++i) {
        tour[i] = static_cast<int>(i + 1);
    }
    return tour;
}

#endif