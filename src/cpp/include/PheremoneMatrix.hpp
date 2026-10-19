#pragma once
#include <vector>
#include <cstddef>
#include <utility>
#include "LowerTriMatrix"

class PheremoneMatrix : public LowerTriMatrix {
    public:
        PheremoneMatrix(size_t dimension, std::vector<double> flat_weights)
            : LowerTriMatrix(dimension, std::move(flat_weights)) {}

        // for pheremone updates
        void set_distance(size_t i, size_t j, double value) {
            if (i == j) return; // no self-loops
            if (i < j) std::swap(i, j); // i > j for lower triangle
            
            size_t index = (i * (i - 1)) / 2 + j;
            weights[index] = value;
        }

        
};