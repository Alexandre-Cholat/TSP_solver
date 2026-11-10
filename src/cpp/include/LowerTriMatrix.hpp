#pragma once
#include <vector>
#include <cstddef>
#include <utility>

class LowerTriMatrix {
    private:
        size_t dim;
        std::vector<double> weights;

    public:

        LowerTriMatrix() = default;
        LowerTriMatrix(size_t dimension, std::vector<double> flat_weights): dim(dimension), weights(std::move(flat_weights)) {}


        size_t get_dimension() const { return dim; }

        double get_distance(size_t i, size_t j) const;

        void set_distance(size_t i, size_t j, double value);

};