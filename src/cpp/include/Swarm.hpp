#pragma once
#include <cstddef>
#include <vector>
#include "PheremoneMatrix.hpp"
#include "LowerTriMatrix.hpp"

class Swarm {
    private:
    // paths taken by each ant in the swarm
        std::vector<std::vector<int>> ant_paths;
        size_t tour_size;
        size_t swarm_size;
        LowerTriMatrix matrix;
        PheremoneMatrix pmatrix;
        std::vector<int> best_tour;
    public:
        Swarm(size_t n, const LowerTriMatrix& ltmatrix);

        void explore();

        void updatePheremones();

        std::vector<int> get_best() const {return best_tour;};

        
        int next_move(int current_node, const std::vector<int>& options);


    
};