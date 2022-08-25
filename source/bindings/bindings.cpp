#include "problem.h"
#include "run.h"
#include "solvers.h"

#include <pybind11/pybind11.h>


PYBIND11_MODULE(pyssym, m)
{
    pyssym::exportProblem(m);

    auto solver = m.def_submodule("Solver");
    pyssym::exportSolvers(solver);

    pyssym::exportRun(m);
}
