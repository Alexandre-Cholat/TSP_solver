#pragma once
#include <memory>
#include <vector>


// flattened 1D std::vector<double> (size $N \times N$) to avoid nested std::vector<std::vector<double>> pointer indirection.

class WeightMatrix{
private:
    /* data */
public:
    WeightMatrix(std::vector<double> matrix);
    ~WeightMatrix() = default;



};

