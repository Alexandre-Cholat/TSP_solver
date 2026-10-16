#ifndef LOWER_TRI_MATRIX_HPP
#define LOWER_TRI_MATRIX_HPP

#include <vector>
#include <algorithm>
#include <cstddef>

class LowerTriMatrix {
private:
    size_t dim;
    std::vector<double> weights;

public:
    LowerTriMatrix(size_t dimension, const std::vector<double>& flat_weights)
        : dim(dimension), weights(flat_weights) {}

    size_t get_dimension() const { return dim; }

    // Map (row, col) queries to 1D lower-triangle index: (i * (i - 1)) / 2 + j
    double get_distance(size_t i, size_t j) const {
        if (i == j) return 0.0;
        if (i < j) std::swap(i, j); // Ensure i > j for lower triangle
        
        size_t index = (i * (i - 1)) / 2 + j;
        return weights[index];
    }
};

#endif