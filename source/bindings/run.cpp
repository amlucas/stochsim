#include "run.h"

#include <ssm/problem.h>
#include <ssm/solvers/solver.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <map>
#include <tuple>
#include <vector>

namespace pyssm {

namespace py = pybind11;
using namespace pybind11::literals;

using namespace ssm;

using TimeSequence = std::vector<real>;
using StatesSequence = std::map<std::string, std::vector<real>>;

static std::tuple<TimeSequence, StatesSequence>
runTrajectory(const Problem& p, StochasticSimulationSolver *solver)
{
    TimeSequence time;
    StatesSequence states;

    const auto speciesNames = p.getSpeciesNames();

    for (const auto& name : speciesNames)
        states[name] = {};

    solver->reset(p.getInitialSpeciesNumbers());

    time.push_back(solver->getTime());

    const auto state = solver->getState();
    for (size_t i = 0; i < speciesNames.size(); ++i)
        states[speciesNames[i]].push_back(state[i]);

    while(solver->getTime() < p.getTend())
    {
        time.push_back(solver->getTime());

        const auto state = solver->getState();
        for (size_t i = 0; i < speciesNames.size(); ++i)
            states[speciesNames[i]].push_back(state[i]);

        solver->advance();
    }

    return {std::move(time),
            std::move(states)};
}

void exportRun(py::module &m)
{
    m.def("run_trajectory", &runTrajectory,
          "problem"_a, "solver"_a);
}

} // namespace pyssm
