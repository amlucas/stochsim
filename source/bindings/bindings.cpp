#include "problem.h"
#include "run.h"
#include "solvers.h"

#include <pybind11/pybind11.h>


PYBIND11_MODULE(pystochsim, m)
{
    pystochsim::exportProblem(m);

    auto solver = m.def_submodule("Solver");
    pystochsim::exportSolvers(solver);

    pystochsim::exportRun(m);
}
