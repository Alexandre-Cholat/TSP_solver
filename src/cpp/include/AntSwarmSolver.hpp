#pragma once
#include "Solver.hpp"
#include <cstddef>
#include <vector>


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
        // build pheremone matrix

        // for desired number of iterations:
            // ants explore N paths semi randomly
            // for N paths, update pheremone matrix according to perfromance
            // save best path
            // repeat
        
        // tour = best path
        return tour;
    }


};