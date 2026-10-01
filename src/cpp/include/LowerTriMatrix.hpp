#pragma once
#include <vector>
#include <cmath>
#include <stdexcept>
#include <utility>

// C++ class to represent a python np array
class LowerTriMatrix {

    private:
    const double* data_;    // raw pointer to NumPy's memory buffer
    size_t num_elements_;   // size of the 1D array
    size_t n_;              // number of nodes (cities) the array represents

    public:
    // Zero-copy constructor taking a pointer to NumPy's buffer
    LowerTriMatrix(const double* data, size_t num_elements)
        : data_(data), num_elements_(num_elements) {
        
        // calculate number of nodes:  N*(N-1)/2 = M  ==>  N^2 - N - 2M = 0
        double n_float = (1.0 + std::sqrt(1.0 + 8.0 * num_elements)) / 2.0;
        n_ = static_cast<size_t>(std::round(n_float));

        if ((n_ * (n_ - 1)) / 2 != num_elements_) {
            throw std::invalid_argument("1D array size does not match number of nodes n_");
        }
    }
    // get distance from node i to another node j
    // Uses inline function: One Definition Rule, Compiler Optimization Hint
    [[nodiscard]] inline double getDistance(size_t i, size_t j) const {
        // check if indices are within array bounds
        if (i >= num_elements_ || j >= num_elements_|| i < 0 || j < 0) {
            throw std::out_of_range("Index out of bounds");
        }

        size_t idx = 0;

        // caclulate i and j intersection idx in flattened lower triangular matrix
        if (i == j) return 0.0;
        if (i < j){
            idx = (j * (j - 1)) / 2 + i;
        }
        else{
            idx = (i * (i - 1)) / 2 + j;
        }

        // return the value from the 1D array
        return data_[idx];
    }

    // get the number of nodes (cities)
    [[nodiscard]] size_t cityCount() const { return n_; }

    // get flattened lower triangular matrix size
    [[nodiscard]] size_t array_size() const { return num_elements_; }


};