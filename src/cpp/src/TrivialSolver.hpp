#include "TrivialSolver.hpp"
#include <cstddef>
#include <vector>

std::vector<int> TrivialSolver:: solve(const LowerTriMatrix& matrix){
    // creating return tour
    std::vector<int> tour;
    size_t nodes = matrix.get_dimension();
    tour.reserve(nodes);
    
    // list 1 , 2, .., dim
    for (int i = 1; i <= static_cast<int>(nodes); ++i) {
        tour.push_back(i);
    }
    
    return tour;
}
