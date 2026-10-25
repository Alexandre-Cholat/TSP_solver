#include <gtest/gtest.h>
#include "TrivialSolver.hpp"
#include <vector>

TEST(TrivialSolverTest, ReturnsSortedVertices) {
    TrivialSolver solver;
    
    std::vector<double> dummy_weights = {0.0, 0.0, 0.0};
    int dimension = 5;

    LowerTriMatrix matrix(dimension, dummy_weights);
    std::vector<int> tour = solver.solve(matrix);

    // should return 1 , 2, ... ,  5
    std::vector<int> expected = {1, 2, 3, 4, 5};
    
    EXPECT_EQ(tour, expected);
}