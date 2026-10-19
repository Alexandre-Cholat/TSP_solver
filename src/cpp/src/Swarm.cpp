#pragma once
#include <cstddef>
#include <vector>
#include "Swarm.hpp"
#include "PheremoneMatrix.hpp"
#include "LowerTriMatrix.hpp"

void Swarm::explore(const LowerTriMatrix& matrix) {
    // For each ant in the swarm, generate a path
    for (std::vector<int>& path : ant_paths) {
        // calculate a path for the ant, considering pheromone levels and distances from the matrix
        for (size_t position = 1; position < path.size(); ++position) {
            // calculate attractivity from matrix options

            // select probalistically based on pheromone levels and attractivity

        }
        
    }
}
