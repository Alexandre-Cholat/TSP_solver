#pragma once
#include <memory>
#include <vector>


// flattened 1D std::vector<double> (size $N \times N$) to avoid nested std::vector<std::vector<double>> pointer indirection.

class WeightMatrix{
private:
    std::vector<double> flat_map;
    size_t map_size;
public:
    // construct from 1D vector
    WeightMatrix(size_t dim, std::vector<double> matrix): map_size(dim), flat_map(std::move(matrix)){};

    //construct from 2D vector
    WeightMatrix(size_t dim, std::vector<std::vector<size_t>> matrix);


    ~WeightMatrix() = default;



};

