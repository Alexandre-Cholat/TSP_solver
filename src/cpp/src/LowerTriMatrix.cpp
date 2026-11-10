#include "LowerTriMatrix.hpp"
#include <vector>
#include <cstddef>
#include <utility>

double LowerTriMatrix::get_distance(size_t i, size_t j) const {
    if (i == j) return 0.0;
    if (i < j) std::swap(i, j); // i > j for lower triangle

    // Convert node idx to 0-based idx for vector reading
    i--;
    j--;
    
    size_t index = (i * (i - 1)) / 2 + j;
    return weights[index];
}

void LowerTriMatrix::set_distance(size_t i, size_t j, double value) {
    if (i == j) return; // distance from i to j allways 0
    if (i < j) std::swap(i, j); // i > j for lower triangle
    
    // Convert node idx to 0-based idx for vector reading
    i--;
    j--;

    size_t index = (i * (i - 1)) / 2 + j;
    weights[index] = value;
}