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
        Swarm(size_t n, const LowerTriMatrix& ltmatrix){
            // set number of ant agents in the swarm
            swarm_size = n;
            ant_paths.resize(swarm_size);
            // set the length of the tour
            tour_size = ltmatrix.get_dimension();
            // copy the distance matrix to the swarm
            matrix = ltmatrix;

            // set length of each path to number of nodes in the distance matrix
            for (std::vector<int>& path : ant_paths) {
                path.resize(tour_size);
            }
            best_tour.resize(tour_size);

            // create empty pheremone matrix based on distance matrix
            pmatrix(ltmatrix);
        }

        void explore();

        void updatePheremones();

        std::vector<int> get_best() const {return best_tour;}

        
        void next_move(size_t position);


    
}