#include "problem.h"

#include <pybind11/pybind11.h>


PYBIND11_MODULE(pyssm, m)
{
    pyssm::exportProblem(m);
}
