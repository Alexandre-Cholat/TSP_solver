#pragma once
#include "Solver.hpp"
#include <cstddef>
#include <vector>

// we inherit from parent class Solver
class HeldKarpSolver : public Solver {
    public:
    std::vector<int> solve(const LowerTriMatrix& matrix) override {
        // creating return tour
        std::vector<int> tour;
        size_t nodes = matrix.get_dimension();
        tour.reserve(nodes);

        //implement held karp
        

        return tour;
    }


};