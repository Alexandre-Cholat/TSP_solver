#include <gtest/gtest.h>
#include "TrivialSolver.hpp"
#include <vector>

TEST(TrivialSolverTest, ReturnsSortedVertices) {
    TrivialSolver solver;
    
    // The trivial solver does not actually use the distance matrix, 
    // so we can pass a dummy array pointer.
    std::vector<double> dummy_weights = {0.0, 0.0, 0.0};
    int dimension = 5;

    std::vector<int> tour = solver.solve(dummy_weights.data(), dimension);

    // The trivial method must return a tour consisting of the vertices sorted by their indices[cite: 3]
    // For an instance with 5 nodes, it should return 1 -> 2 -> 3 -> 4 -> 5[cite: 3]
    std::vector<int> expected = {1, 2, 3, 4, 5};
    
    EXPECT_EQ(tour, expected);
}