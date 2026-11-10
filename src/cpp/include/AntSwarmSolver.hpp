#pragma once
#include "Solver.hpp"
#include <cstddef>


// ant colony optimization algorithm
// https://en.wikipedia.org/wiki/Ant_colony_optimization_algorithms

class AntSwarmSolver : public Solver {
    public:
    
    // default: alpha = 1, beta = 3, swarm_size = 1000, iterations = 100
    std::vector<int> solve(const LowerTriMatrix& matrix) override {return solve(matrix, 1.0, 3.0, 1000, 100); }
    
    std::vector<int> solve(const LowerTriMatrix& matrix, double alpha, double beta, size_t swarm_size, size_t maxIt);

};