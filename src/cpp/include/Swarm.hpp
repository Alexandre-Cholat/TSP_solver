#pragma once
#include <cstddef>
#include <vector>
#include "PheremoneMatrix.hpp"
#include "LowerTriMatrix.hpp"

class Swarm {
    private:
    // paths taken by each ant in the swarm
        std::vector<std::vector<int>> ant_paths;
        PheremoneMatrix pmatrix;
    public:
        Swarm(size_t n, const LowerTriMatrix& ltmatrix){
            // set number of ant agents in the swarm
            ant_paths.resize(n);

            // set length of each path to number of nodes in the distance matrix
            for (std::vector<int>& path : ant_paths) {
                path.resize(ltmatrix.get_dimension());
            }

            // create empty pheremone matrix based on distance matrix
            pmatrix(ltmatrix);
        }

        void explore(const LowerTriMatrix& matrix);

        void updatePheremones();


    
}