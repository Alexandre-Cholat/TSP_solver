#pragma once
#include "Solver.hpp"
#include <cstddef>
#include <vector>

// we inherit from parent class Solver
class TrivialSolver : public Solver {
public:

    std::vector<int> solve(const LowerTriMatrix& matrix) override {

        // creating return vector tour
        std::vector<int> tour;
        size_t nodes = matrix.get_dimension();
        tour.reserve(nodes);
        
        // fill tour with list of nodes
        for (int i = 1; i <= static_cast<int>(nodes); ++i) {
            tour.push_back(i);
        }
        
        return tour;
    }
};