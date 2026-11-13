#include "HeldKarpSolver.hpp"
#include "Solver.hpp"
#include <cstddef>
#include <vector>
#include <limits>
#include <algorithm>


std::vector<int> HeldKarpSolver:: solve(const LowerTriMatrix& matrix) {
    size_t n = matrix.get_dimension();
    if (n == 0) return {};
    if (n == 1) return {1};

    const double INF = std::numeric_limits<double>::infinity();
    size_t num_states = 1ULL << n;

    // DP state: dp[mask][u] stores min distance starting at node 0, visiting subset 'mask', ending at 'u'
    std::vector<std::vector<double>> dp(num_states, std::vector<double>(n, INF));
    std::vector<std::vector<int>> parent(num_states, std::vector<int>(n, -1));

    // Base case: start at node 0 (1-based ID: 1)
    dp[1][0] = 0.0;

    for (size_t mask = 1; mask < num_states; ++mask) {
        // Must contain start node 0
        if (!(mask & 1)) continue;

        for (size_t u = 0; u < n; ++u) {
            if (dp[mask][u] == INF) continue;

            for (size_t v = 0; v < n; ++v) {
                if (!(mask & (1ULL << v))) {
                    size_t next_mask = mask | (1ULL << v);
                    double dist = matrix.get_distance(u + 1, v + 1);
                    double new_cost = dp[mask][u] + dist;

                    if (new_cost < dp[next_mask][v]) {
                        dp[next_mask][v] = new_cost;
                        parent[next_mask][v] = static_cast<int>(u);
                    }
                }
            }
        }
    }

    // Return to start node 0 from the last visited node
    size_t full_mask = num_states - 1;
    double min_cost = INF;
    int last_node = -1;

    for(size_t u = 1; u < n; ++u){
        if (dp[full_mask][u] != INF) {
            double total_cost = dp[full_mask][u] + matrix.get_distance(u + 1, 1);
            if (total_cost < min_cost) {
                min_cost = total_cost;
                last_node = static_cast<int>(u);
            }
        }
    }

    if (last_node == -1) return {};

    // Reconstruct path
    std::vector<int> tour;
    tour.reserve(n);

    size_t curr_mask = full_mask;
    int curr_node = last_node;

    while (curr_node != -1) {
        tour.push_back(curr_node + 1); // convert to 1-based node ID
        int p = parent[curr_mask][curr_node];
        curr_mask ^= (1ULL << curr_node);
        curr_node = p;
    }

    std::reverse(tour.begin(), tour.end());

    return tour;
}