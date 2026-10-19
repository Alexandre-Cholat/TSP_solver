#pragma once
#include "Swarm.hpp"
#include <cstddef>
#include <vector>
#include "PheremoneMatrix.hpp"
#include "LowerTriMatrix.hpp"

void Swarm::explore() {
    // clear ant_paths
    for (std::vector<int>& path : ant_paths) {
        std::fill(path.begin(), path.end(), -1); // reset to -1 or any invalid value
    }

    // For each ant in the swarm
    for (size_t agent = 0; agent < swarm_size; agent++) {
        // calculate her move iteratively for each position in the tour.
        
        // options vector, the list of nodes (2 - tour_size) that have not been visited yet
        // this choice assumes we will never revisit a city
        std::vector<int> options(tour_size);
        for (size_t i = 2; i < tour_size; i++) {
            options[i-2] = i;
        }
        // starting node is always 0
        for (size_t position = 1; position < tour_size - 1; position++) {
            
            // calculate next move for the ant, considering current position, pheromone levels and distances from the matrix
            ant_paths[agent][position+1] = next_move(position, options);

            // remove next move from options
            options.erase(std::remove(options.begin(), options.end(), ant_paths[agent][position]), options.end());
        }
        // last move is allways to return to the starting node
        ant_paths[agent][tour_size-1] = ant_paths[agent][0];
        
    }
}

int Swarm::next_move(size_t position, const std::vector<int>& options) {
    // calculate attractivity from options
    for(int o : options) {
        // attractivity = 1 / distance from current position to option

        
        // distance = matrix.get_distance(current_position, option)

    }

    // select probalistically based on pheromone levels and attractivity

    // return selected node index in integer type
    return 0; // Placeholder
}
