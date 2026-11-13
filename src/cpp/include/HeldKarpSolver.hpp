#pragma once
#include "Solver.hpp"
#include <cstddef>
#include <vector>
#include <limits>
#include <algorithm>

// inherited from parent class Solver
class HeldKarpSolver : public Solver {
public:
    std::vector<int> solve(const LowerTriMatrix& matrix) override;
};