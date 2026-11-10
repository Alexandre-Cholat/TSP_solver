#include "AntSwarmSolver.hpp"
#include "Solver.hpp"
#include <cstddef>
#include <iostream>
#include <vector>
#include "Swarm.hpp"

std::vector<int> AntSwarmSolver:: solve(const LowerTriMatrix& matrix, double alpha, double beta, size_t swarm_size, size_t maxIt){
    // creating return tour
    std::vector<int> tour;
    size_t nodes = matrix.get_dimension();
    tour.reserve(nodes);
    
    // build an ant swarm of swarm_size agents
    Swarm ants(swarm_size, matrix);

    //set hyperparameters
    // If α= 0, the closest cities are selected (greedy)
    // If β= 0, only pheromone ampliﬁcation is at work
    ants.set_alpha(alpha);
    ants.set_beta(beta);

    // for desired number of iterations maxIt:
    for(size_t iteration = 0; iteration <= maxIt; ++iteration){
        // ants explore N paths semi randomly
        ants.explore();
        // for N paths, update pheremone matrix according to perfromance
        ants.updatePheremones();
        // repeat
        std::cout << "Iteration "<< iteration << " of swarm, shortest tour = " << ants.get_best_length() << " .\n";
    }
    std::cout << "Length of shortest tour : "<<  ants.get_best_length() << " .\n";
    
    return ants.get_best();
}