#pragma once
#include <vector>
#include "LowerTriMatrix.hpp"

class Solver {
public:
    virtual ~Solver() = default;

    // Pass the raw pointer and size to avoid copying the matrix.
    virtual std::vector<int> solve(const LowerTriMatrix& matrix) = 0;
};