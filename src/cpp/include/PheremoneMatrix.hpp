#pragma once
#include <vector>
#include <cstddef>
#include <utility>
#include "LowerTriMatrix.hpp"

class PheremoneMatrix : public LowerTriMatrix {
    public:
        PheremoneMatrix() = default;

        PheremoneMatrix(size_t dimension, std::vector<double> flat_weights): LowerTriMatrix(dimension, std::move(flat_weights)) {}

        // Constructor that initializes the pheromone matrix based on a given LowerTriMatrix
        PheremoneMatrix(const LowerTriMatrix& matrix);
        
        // to replace get_pheromone for clarity
        double get_pheromone(size_t i, size_t j) const {
            return get_distance(i, j);
        }

        // for pheremone updates
        void set_pheromone(size_t i, size_t j, double value) {
            set_distance( i, j, value);
        }

};