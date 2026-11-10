#include <gtest/gtest.h>
#include "TrivialSolver.hpp"
#include "AntSwarmSolver.hpp"
#include "HeldKarpSolver.hpp"
#include <vector>

TEST(TrivialSolverTest, ReturnsSortedVertices) {
    TrivialSolver solver;
    
    std::vector<double> dummy_weights = {0.0, 0.0, 0.0};
    int dimension = 5;
    LowerTriMatrix matrix(dimension, dummy_weights);
    std::vector<int> tour = solver.solve(matrix);

    std::vector<int> expected = {1, 2, 3, 4, 5};
    
    EXPECT_EQ(tour, expected);
}

TEST(AntSwarmSolverTest, SingleNodeEdgeCase) {
    AntSwarmSolver solver;
    LowerTriMatrix matrix(1, {});
    std::vector<int> tour = solver.solve(matrix);

    ASSERT_EQ(tour.size(), 1u);
    EXPECT_EQ(tour[0], 1);
}

TEST(AntSwarmSolverTest, DoubleNodeEdgeCase) {
    AntSwarmSolver solver;
    LowerTriMatrix matrix(2, {42.0});
    std::vector<int> tour = solver.solve(matrix);

    std::sort(tour.begin(), tour.end());
    std::vector<int> expected = {1, 2};
    EXPECT_EQ(tour, expected);
}

TEST(AntSwarmSolverTest, TestCase) {
    AntSwarmSolver solver;
    size_t dimension = 5;
    std::vector<double> weights = {
        10.0,
        15.0, 20.0,
        25.0, 30.0, 35.0,
        40.0, 45.0, 50.0, 55.0
    };
    LowerTriMatrix matrix(dimension, weights);
    std::vector<int> tour = solver.solve(matrix);

    std::sort(tour.begin(), tour.end());
    std::vector<int> expected = {1, 2, 3, 4, 5};
    EXPECT_EQ(tour, expected);
}