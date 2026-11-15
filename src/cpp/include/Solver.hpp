#pragma once
#include <vector>
#include "LowerTriMatrix.hpp"

class Solver {
public:
    virtual ~Solver() = default;

    virtual std::vector<int> solve(const LowerTriMatrix& matrix) = 0;
};