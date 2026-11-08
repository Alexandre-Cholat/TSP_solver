#include "Swarm.hpp"
#include <cstddef>
#include <vector>
#include <algorithm>
#include "PheremoneMatrix.hpp"
#include "LowerTriMatrix.hpp"
#include <random>

Swarm:: Swarm(size_t n, const LowerTriMatrix& ltmatrix){
    // set number of ant agents in the swarm
    swarm_size = n;
    ant_paths.resize(swarm_size);
    // set the length of the tour
    tour_size = ltmatrix.get_dimension();
    // copy the distance matrix to the swarm
    matrix = ltmatrix;

    // set length of each path to number of nodes +1 (+1 for return to starting node 0) in the distance matrix
    for (std::vector<int>& path : ant_paths) {
        path.resize(tour_size + 1);
    }
    best_tour.resize(tour_size + 1);

    // create pheremone matrix to match based on distance matrix
    pmatrix = PheremoneMatrix(ltmatrix);
}


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
        std::vector<int> options(tour_size-1);
        for (size_t i = 1; i < tour_size; i++) {
            options[i-1] = i;
        }

        // starting node is always 0
        int current_node = 0;
        ant_paths[agent][0] = current_node;

        for (size_t position = 1; position < tour_size; position++) {
            
            // calculate next move for the ant, considering current position, pheromone levels and distances from the matrix
            int chosen_node = next_move(current_node, options);

            ant_paths[agent][position] = chosen_node;

            // remove next move from options
            options.erase(std::remove(options.begin(), options.end(), chosen_node), options.end());
            current_node = chosen_node;
        }
        // last move is allways to return to the starting node
        ant_paths[agent][tour_size] = ant_paths[agent][0];
        
    }
}

int Swarm::next_move(int current_node, const std::vector<int>& options) {

    // create a weight vector based on pheromone levels and distances
    std::vector<double> weights(options.size());
    for (size_t i = 0; i < options.size(); i++) {

        // penalize large distances and reward high pheromone levels
        weights[i] = pmatrix.get_pheromone(current_node, options[i]) / matrix.get_distance(current_node, options[i]);
    }

    // build a discrete distribution based on the weights
    std::discrete_distribution<int> dist(weights.begin(), weights.end());

    //random number generator
    std::random_device rd;
    std::mt19937 gen(rd());

    // select probalistically based on weight
    int index = dist(gen);

    // return selected node index in integer type
    return options[index];
}

// Trails are usually updated when all ants have completed their solution, 
// increasing or decreasing the level of trails corresponding to moves that were "good" or "bad" solutions

void Swarm::updatePheremones() {
    // for each ant path, update pheromone levels based on the path length
    for (const auto& path : ant_paths) {
        
        // calculate the total length of the path
        double path_length = 0.0;
        for (size_t i = 0; i < tour_size - 1; i++) {
            path_length += matrix.get_distance(path[i], path[i + 1]);
        }

        // TODO: Implement pheromone evaporation here 

    

        // update pheromone levels for nodes on that path inversely proportional to the path length
        // pheremones 
        for (size_t i = 0; i < tour_size - 1; i++) {
            double current_pheromone = pmatrix.get_pheromone(path[i], path[i + 1]);
            pmatrix.set_pheromone(path[i], path[i + 1], current_pheromone + (1.0 / path_length));
        }
    }
}
