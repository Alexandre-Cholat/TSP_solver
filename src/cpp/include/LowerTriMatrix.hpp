#pragma once
#include <vector>
#include <cstddef>
#include <utility>

class LowerTriMatrix {
    private:
        size_t dim;
        std::vector<double> weights;

    public:
        LowerTriMatrix(size_t dimension, std::vector<double> flat_weights)
            : dim(dimension), weights(std::move(flat_weights)) {}

        size_t get_dimension() const { return dim; }

        double get_distance(size_t i, size_t j) const {
            if (i == j) return 0.0;
            if (i < j) std::swap(i, j); // i > j for lower triangle
            
            size_t index = (i * (i - 1)) / 2 + j;
            return weights[index];
        }
};