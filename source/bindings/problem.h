#pragma once

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace pyssm {

void exportProblem(py::module &m);

}  // namespace pyssm
