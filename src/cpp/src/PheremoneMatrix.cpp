#include "PheremoneMatrix.hpp"
#include <vector>
#include <cstddef>
#include <utility>
#include "LowerTriMatrix.hpp"

PheremoneMatrix:: PheremoneMatrix(const LowerTriMatrix& matrix): LowerTriMatrix(matrix.get_dimension(), std::vector<double>(matrix.get_dimension() * (matrix.get_dimension() - 1) / 2)) {
            // Initialize pheromone values to a default value 1.0 everywhere:
            for (size_t i = 1; i <= get_dimension(); ++i) {
                for (size_t j = 1; j < i; ++j) {
                    set_pheromone(i, j, 1.0);
                }
            }
        }