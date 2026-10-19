#pragma once
#include <vector>
#include <cstddef>
#include <utility>
#include "LowerTriMatrix"

class PheremoneMatrix : public LowerTriMatrix {
    public:
        PheremoneMatrix(size_t dimension, std::vector<double> flat_weights)
            : LowerTriMatrix(dimension, std::move(flat_weights)) {}

        // Constructor that initializes the pheromone matrix based on a given LowerTriMatrix
        PheremoneMatrix(const LowerTriMatrix& matrix)
            : LowerTriMatrix(matrix.get_dimension(), std::vector<double>(matrix.get_dimension() * (matrix.get_dimension() - 1) / 2)) {
                // Initialize pheromone values to a default value 1.0 everywhere:
                for (size_t i = 0; i < get_dimension(); ++i) {
                    for (size_t j = 0; j < i; ++j) {
                        set_distance(i, j, 1.0);
                    }
                }
            }

        // for pheremone updates
        void set_distance(size_t i, size_t j, double value) {
            if (i == j) return; // no self-loops
            if (i < j) std::swap(i, j); // i > j for lower triangle
            
            size_t index = (i * (i - 1)) / 2 + j;
            weights[index] = value;
        }

        
};