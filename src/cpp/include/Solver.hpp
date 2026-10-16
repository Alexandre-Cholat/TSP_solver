#pragma once
#include <vector>
#include <cstddef> // for std::size_t

class Solver {
public:
    virtual ~Solver() = default;

    // Pass the raw pointer and size to avoid copying the matrix.
    virtual std::vector<int> solve(const double* mat, std::size_t nodes) = 0;
};