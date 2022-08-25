#include "problem.h"
#include "run.h"
#include "solvers.h"

#include <pybind11/pybind11.h>


PYBIND11_MODULE(pyssm, m)
{
    pyssm::exportProblem(m);

    auto solver = m.def_submodule("Solver");
    pyssm::exportSolvers(solver);

    pyssm::exportRun(m);
}
