#include "Solver.hpp"
#include <cstddef>
#include <vector>

// we inherit from parent class Solver
class TrivialSolver : public Solver {
    public:

    std::vector<int> solve(const LowerTriMatrix& matrix) override;
};