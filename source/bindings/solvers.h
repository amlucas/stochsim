#pragma once

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace pyssm {

void exportSolvers(py::module &m);

}  // namespace pyssm