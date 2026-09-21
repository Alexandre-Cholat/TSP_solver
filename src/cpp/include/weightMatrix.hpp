#pragma once
#include <memory>
#include <vector>


// flattened 1D std::vector<double> (size $N \times N$) to avoid nested std::vector<std::vector<double>> pointer indirection.

class weightMatrix{
private:
    /* data */
public:
    weightMatrix(std::vector<double> matrix);
    ~weightMatrix() = default;



};

