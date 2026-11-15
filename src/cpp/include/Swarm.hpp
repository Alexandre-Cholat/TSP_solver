#pragma once
#include <cstddef>
#include <vector>
#include "PheremoneMatrix.hpp"
#include "LowerTriMatrix.hpp"

class Swarm {

    // Reference: Stützle, Thomas & Dorigo, Marco. (1999). ACO Algorithms for the Traveling Salesman Problem. 

    private:
        std::vector<std::vector<int>> ant_paths;
        size_t tour_size;
        size_t swarm_size;
        LowerTriMatrix matrix;
        PheremoneMatrix pmatrix;

        // best path trackers
        std::vector<int> best_tour;
        double best_length = 1e18; // track shortest path length

        // hyper parameters
        double alpha;
        double beta;

    public:
        Swarm(size_t n, const LowerTriMatrix& ltmatrix);

        void explore();

        void updatePheremones();

        int next_move(int current_node, const std::vector<int>& options);

        std::vector<int> get_best() const {return best_tour;};
        double get_best_length() const {return best_length;};

        void set_alpha(double al) {alpha = al;};
        void set_beta(double bet) {beta = bet;};
        
};