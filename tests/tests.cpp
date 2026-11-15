#include <gtest/gtest.h>
#include "TrivialSolver.hpp"
#include "AntSwarmSolver.hpp"
#include "HeldKarpSolver.hpp"
#include <vector>

// Helper to compute total round-trip tour cost
static double compute_tour_cost(const LowerTriMatrix& matrix, const std::vector<int>& tour) {
    if (tour.size() <= 1) return 0.0;
    double cost = 0.0;
    for (size_t i = 0; i < tour.size(); ++i) {
        int u = tour[i];
        int v = tour[(i + 1) % tour.size()];
        cost += matrix.get_distance(u, v);
    }
    return cost;
}

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


TEST(HeldKarpSolverTest, EdgeCasesEmptyAndSingleNode) {
    HeldKarpSolver solver;

    LowerTriMatrix empty_matrix(0, {});
    EXPECT_TRUE(solver.solve(empty_matrix).empty());

    LowerTriMatrix single_matrix(1, {});
    std::vector<int> tour = solver.solve(single_matrix);
    ASSERT_EQ(tour.size(), 1u);
    EXPECT_EQ(tour[0], 1);
}

TEST(HeldKarpSolverTest, DoubleNodeEdgeCase) {
    HeldKarpSolver solver;
    LowerTriMatrix matrix(2, {42.0});
    std::vector<int> tour = solver.solve(matrix);

    std::vector<int> expected = {1, 2};
    EXPECT_EQ(tour, expected);
    EXPECT_DOUBLE_EQ(compute_tour_cost(matrix, tour), 84.0);
}

TEST(HeldKarpSolverTest, FindsExactOptimumFourNodes) {
    HeldKarpSolver solver;
    std::vector<double> weights = {
        1.0,              
        10.0, 1.0,        
        1.0, 10.0, 1.0    
    };
    LowerTriMatrix matrix(4, weights);
    std::vector<int> tour = solver.solve(matrix);

    ASSERT_EQ(tour.size(), 4u);
    EXPECT_EQ(tour[0], 1); 
    EXPECT_DOUBLE_EQ(compute_tour_cost(matrix, tour), 4.0);

    std::sort(tour.begin(), tour.end());
    std::vector<int> expected_nodes = {1, 2, 3, 4};
    EXPECT_EQ(tour, expected_nodes);
}

TEST(HeldKarpSolverTest, AvoidsGreedyTrapFiveNodes) {
    HeldKarpSolver solver;
    std::vector<double> weights = {
        2.0,                    
        1.0, 2.0,               
        20.0, 20.0, 2.0,        
        2.0, 20.0, 20.0, 2.0    
    };
    LowerTriMatrix matrix(5, weights);
    std::vector<int> tour = solver.solve(matrix);

    ASSERT_EQ(tour.size(), 5u);
    EXPECT_DOUBLE_EQ(compute_tour_cost(matrix, tour), 10.0);

    std::sort(tour.begin(), tour.end());
    std::vector<int> expected_nodes = {1, 2, 3, 4, 5};
    EXPECT_EQ(tour, expected_nodes);
}