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
        const size_t N = 10; // number of ants
        Swarm ants(N, matrix);

        // for desired number of iterations:
        for (size_t iteration = 0; iteration < 15; ++iteration) {
            // ants explore N paths semi randomly
            ants.explore();
            // for N paths, update pheremone matrix according to perfromance
            ants.updatePheremones();
            // retrieve the best path
            tour = ants.get_best()
            // repeat
        }
        return tour;
    }


};