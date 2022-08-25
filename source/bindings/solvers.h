#pragma once

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace pystochsim {

void exportSolvers(py::module &m);

}  // namespace pystochsim
