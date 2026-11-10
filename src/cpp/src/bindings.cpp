#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <memory>
#include <vector>
#include "parsers.hpp"
#include "TrivialSolver.hpp"
#include "AntSwarmSolver.hpp"
#include "HeldKarpSolver.hpp"

// to use pybind11, must have venv, and select -DBUILD_PYTHON=ON option manualy instead of using bash script
// .venv/bin/activate
// cd build
// cmake -DBUILD_PYTHON=ON ..
// cmake --build .

// This file is intended to provide Python bindings for the TSP solver. It uses pybind11 to expose the C++ functionality to Python. 
// We originally planned to run solver from python, call cpp for fast computing, then return result to Python.
// Upon further inspection of TSP isntructuons, we realized that 
// bindings.cpp is omitted from the add_executable source list to prevent conflicts: add to CMakeLists.txt to use this.

namespace py = pybind11;

// std::vector<int> solve_tsp(py::array_t<double> array) {
//     py::buffer_info info = array.request();
//     double* data = static_cast<double*>(info.ptr);
    
//     size_t nodes = static_cast<size_t>(data[0]);
//     int algo_type = static_cast<int>(data[1]);
//     double* mat = data + 2; 

//     std::unique_ptr<Solver> solver;

//     if(algo_type == 1) {
//         solver = std::make_unique<TrivialSolver>();
//     }
//     // add more algorithms here as needed
//     else {
//         throw std::invalid_argument("Unknown algo type");
//     }

//     // Pass the raw pointer to avoid copying the matrix
//     return solver->solve(mat, nodes); 
// }

PYBIND11_MODULE(tsp_bindings, m) {
    // python to c++
    // m.def("solve_tsp", &solve_tsp, "Solve TSP given a flattened lower matrix array");



    // c++ to python

    // Expose LowerTriMatrix
    py::class_<LowerTriMatrix>(m, "LowerTriMatrix")
        .def(py::init<size_t, std::vector<double>>())
        .def("get_dimension", &LowerTriMatrix::get_dimension)
        .def("get_distance", &LowerTriMatrix::get_distance);

    // Expose Parsers
    py::class_<CoordParser>(m, "CoordParser")
        .def(py::init<const std::string&>())
        .def("parse", &CoordParser::parse);

    py::class_<LowerTriParser>(m, "LowerTriParser")
        .def(py::init<const std::string&>())
        .def("parse", &LowerTriParser::parse);

    // Expose Solvers
    py::class_<TrivialSolver>(m, "TrivialSolver")
        .def(py::init<>())
        .def("solve", &TrivialSolver::solve);

    py::class_<AntSwarmSolver>(m, "AntSwarmSolver")
        .def(py::init<>())
        .def("solve", &AntSwarmSolver::solve);

    py::class_<HeldKarpSolver>(m, "HeldKarpSolver")
        .def(py::init<>())
        .def("solve", &HeldKarpSolver::solve);
}