#include "solvers.h"

#include <stochsim/problem.h>
#include <stochsim/solvers/R_leaping.h>
#include <stochsim/solvers/solver.h>
#include <stochsim/solvers/ssa.h>
#include <stochsim/solvers/tau_leaping.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace pystochsim {

namespace py = pybind11;
using namespace pybind11::literals;

using namespace stochsim;

void exportSolvers(py::module &m)
{
    py::class_<StochasticSimulationSolver> pySolver(m, "StochasticSimulationSolver");

    py::class_<SSA>(m, "SSA", pySolver)
        .def(py::init([](const Problem& p)
                      {return SSA(p.getTend(), p.getReactions(), p.getInitialSpeciesNumbers());}),
                "problem"_a);


    py::class_<TauLeaping>(m, "TauLeaping", pySolver)
        .def(py::init([](const Problem& p,
                         int nc, real eps, real acceptFactor, int numStepsSSA)
                      {return TauLeaping(p.getTend(), nc, eps, acceptFactor, numStepsSSA,
                                         p.getReactions(), p.getInitialSpeciesNumbers());}),
             "problem"_a, "nc"_a=10, "eps"_a=0.03_r, "accept_factor"_a=10.0_r, "num_steps_SSA"_a=100);

    py::class_<RLeaping>(m, "RLeaping", pySolver)
        .def(py::init([](const Problem& p,
                         real eps, real theta, int sortingPeriod)
                      {return RLeaping(p.getTend(), eps, theta, sortingPeriod,
                                       p.getReactions(), p.getInitialSpeciesNumbers());}),
             "problem"_a, "eps"_a=0.03_r, "theta"_a=0.4_r, "sorting_period"_a=50);
}

} // namespace pystochsim
