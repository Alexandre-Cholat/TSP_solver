#pragma once
#include "Solver.hpp"
#include <cstddef>
#include <vector>
#include "Swarm.hpp"

// ant colony optimization algorithm
// https://en.wikipedia.org/wiki/Ant_colony_optimization_algorithms
class AntSwarmSolver : public Solver {
    public:
    std::vector<int> solve(const LowerTriMatrix& matrix) override {
        // creating return tour
        std::vector<int> tour;
        size_t nodes = matrix.get_dimension();
        tour.reserve(nodes);
        
        // build an ant swarm of N agents
        const size_t N = 1000;
        Swarm ants(N, matrix);

        //set hyperparameters
        ants.set_alpha(2);
        ants.set_beta(3);

        // for desired number of iterations maxIt:
        size_t maxIt = 15;
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


};