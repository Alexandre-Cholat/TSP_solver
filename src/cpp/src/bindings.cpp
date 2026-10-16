#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // Enables automatic conversion between STL types and Python
#include <vector>

namespace py = pybind11;

// test function
py::array_t<double> process_pybind11(py::array_t<double> arr) {
    py::buffer_info in_buf = arr.request();
    double* in_ptr = static_cast<double*>(in_buf.ptr);

    // Create a new NumPy array of the exact same shape
    py::array_t<double> result(in_buf.shape);
    py::buffer_info out_buf = result.request();
    double* out_ptr = static_cast<double*>(out_buf.ptr);

    // Fill the new array
    for (ssize_t i = 0; i < in_buf.size; ++i) {
        out_ptr[i] = in_ptr[i] + 10.0;
    }

    // Return the new array to Python
    return result;
}

py::array_t<double> process_trivial(py::array_t<double> array){
    // convert array to vector------- no!! info is a pointer!!
    py::buffer_info info = array.request();
    double* data = static_cast<double*>(info.ptr);

    // retrive the amount of nodes, passed as first element in runner.py
    size_t nodes = data[0];
    
    // build Solver
    solver = std::make_unique<Trivial>();
    return solver->solve(mat, nodes);
} 

// This macro exposes the functions to Python under the name 'fast_engine'
PYBIND11_MODULE(fast_engine, m) {
    m.def("process_pybind11", &process_pybind11);
    m.def("process_trivial", &process_trivial);
}