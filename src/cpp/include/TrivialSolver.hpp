#pragma once
#include "Solver.hpp"
#include <cstddef>
#include <vector>

// we inherit from parent class Solver
class TrivialSolver : public Solver {
public:

    std::vector<int> solve(const double* mat, std::size_t nodes) override {
        // mat is a pointer to the flattened adjacency matrix, not needed here.
        std::vector<int> tour;
        tour.reserve(nodes);
        
        // returns the tour consisting of the vertices sorted by there indices.
        for (int i = 1; i <= static_cast<int>(nodes); ++i) {
            tour.push_back(i);
        }
        
        return tour;
    }
};