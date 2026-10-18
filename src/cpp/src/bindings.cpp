#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h> // Required to auto-convert std::vector to Python lists
#include <memory>
#include <vector>
#include "TrivialSolver.hpp"
#include "HeldKarpSolver.hpp"

// This file is intended to provide Python bindings for the TSP solver. It uses pybind11 to expose the C++ functionality to Python. 
// We originally planned to run solver from python, call cpp for fast computing, then return result to Python.
// Upon further inspection of TSP isntructuons, we realized that 
// bindings.cpp is omitted from the add_executable source list to prevent conflicts: add to CMakeLists.txt to use this.

namespace py = pybind11;

std::vector<int> solve_tsp(py::array_t<double> array) {
    py::buffer_info info = array.request();
    double* data = static_cast<double*>(info.ptr);
    
    size_t nodes = static_cast<size_t>(data[0]);
    int algo_type = static_cast<int>(data[1]);
    double* mat = data + 2; 

    std::unique_ptr<Solver> solver;

    if(algo_type == 1) {
        solver = std::make_unique<TrivialSolver>();
    }
    elif(algo_type == 2) {
        solver = std::make_unique<HeldKarpSolver>();
    }
    // add more algorithms here as needed
    else {
        throw std::invalid_argument("Unknown algo type");
    }

    // Pass the raw pointer to avoid copying the matrix
    return solver->solve(mat, nodes); 
}

PYBIND11_MODULE(tsp_bindings, m) {
    m.def("solve_tsp", &solve_tsp, "Solve TSP given a flattened lower matrix array");
}