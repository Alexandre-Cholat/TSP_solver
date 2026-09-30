#pragma once
#include <vector>
#include <cmath>
#include <stdexcept>
#include <utility>

class LowerTriMatrix {
private:
    const double* data_;
    size_t num_elements_;
    size_t n_;

    public:
    // Zero-copy constructor taking a pointer to NumPy's buffer
    LowerTriMatrix(const double* data, size_t num_elements)
        : data_(data), num_elements_(num_elements) {
        
        // Solve N*(N-1)/2 = M  ==>  N^2 - N - 2M = 0
        double n_float = (1.0 + std::sqrt(1.0 + 8.0 * num_elements)) / 2.0;
        n_ = static_cast<size_t>(std::round(n_float));

        if ((n_ * (n_ - 1)) / 2 != num_elements_) {
            throw std::invalid_argument("1D array size does not match N*(N-1)/2");
        }
    }

    [[nodiscard]] inline double get(size_t i, size_t j) const noexcept {
        if (i == j) return 0.0;
        if (i < j) std::swap(i, j); // Ensure i > j
        
        // Lower-triangular index: i * (i - 1) / 2 + j
        size_t idx = (i * (i - 1)) / 2 + j;
        return data_[idx];
    }

    [[nodiscard]] size_t size() const noexcept { return n_; }
    [[nodiscard]] size_t buffer_size() const noexcept { return num_elements_; }


};