#include <gtest/gtest.h>
#include <vector>
#include "LowerTriMatrix.hpp"
#include "algorithms/trivial.hpp"

// Test 1: Verify LowerTriMatrix distance lookup
TEST(LowerTriMatrixTest, DistanceLookup) {
    // 3 cities: dim = 3
    // Lower triangle weights for pairs: (1,0)=5.0, (2,0)=8.0, (2,1)=4.0
    size_t dim = 3;
    std::vector<double> flat_weights = {5.0, 8.0, 4.0};

    LowerTriMatrix matrix(dim, flat_weights);

    // Distance to self should be 0
    EXPECT_DOUBLE_EQ(matrix.get_distance(0, 0), 0.0);
    EXPECT_DOUBLE_EQ(matrix.get_distance(1, 1), 0.0);

    // Distance queries should be symmetric: d(i,j) == d(j,i)
    EXPECT_DOUBLE_EQ(matrix.get_distance(1, 0), 5.0);
    EXPECT_DOUBLE_EQ(matrix.get_distance(0, 1), 5.0);

    EXPECT_DOUBLE_EQ(matrix.get_distance(2, 0), 8.0);
    EXPECT_DOUBLE_EQ(matrix.get_distance(0, 2), 8.0);

    EXPECT_DOUBLE_EQ(matrix.get_distance(2, 1), 4.0);
    EXPECT_DOUBLE_EQ(matrix.get_distance(1, 2), 4.0);
}

// Test 2: Verify Trivial solver generates sequence 1 -> 2 -> ... -> N
TEST(TrivialAlgorithmTest, PermutationGeneration) {
    size_t dim = 5;
    std::vector<int> tour = solve_trivial(dim);

    ASSERT_EQ(tour.size(), dim);
    for (size_t i = 0; i < dim; ++i) {
        EXPECT_EQ(tour[i], static_cast<int>(i + 1));
    }
}

// Test 3: Verify tour distance calculation
TEST(TrivialAlgorithmTest, TourLengthCalculation) {
    size_t dim = 3;
    std::vector<double> flat_weights = {5.0, 8.0, 4.0}; // d(1,0)=5, d(2,0)=8, d(2,1)=4
    LowerTriMatrix matrix(dim, flat_weights);

    std::vector<int> tour = {1, 2, 3}; // Tour: 1 -> 2 -> 3 -> 1 (node 1=idx 0, node 2=idx 1, node 3=idx 2)
    
    // Path:
    // 1 -> 2 (idx 0 to 1): 5.0
    // 2 -> 3 (idx 1 to 2): 4.0
    // 3 -> 1 (idx 2 to 0): 8.0
    // Total = 5.0 + 4.0 + 8.0 = 17.0
    double total_length = calculate_tour_length(matrix, tour);
    EXPECT_DOUBLE_EQ(total_length, 17.0);
}