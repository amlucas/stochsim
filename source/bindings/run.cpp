#include "run.h"

#include <stochsim/problem.h>
#include <stochsim/solvers/solver.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <map>
#include <tuple>
#include <vector>

namespace pystochsim {

namespace py = pybind11;
using namespace pybind11::literals;

using namespace stochsim;

using TimeSequence = std::vector<real>;
using StatesSequence = std::map<std::string, std::vector<real>>;

static std::tuple<TimeSequence, StatesSequence>
runTrajectory(const Problem& p,
              StochasticSimulationSolver *solver,
              real minDeltatDump=0.0_r)
{
    TimeSequence time;
    StatesSequence states;

    const auto speciesNames = p.getSpeciesNames();

    for (const auto& name : speciesNames)
        states[name] = {};

    solver->reset(p.getInitialSpeciesNumbers());

    auto recordState = [&]()
    {
        time.push_back(solver->getTime());

        const auto state = solver->getState();
        for (size_t i = 0; i < speciesNames.size(); ++i)
            states[speciesNames[i]].push_back(state[i]);
    };

    real lastDumpTime = -minDeltatDump - 1.0_r;

    while(solver->getTime() < p.getTend())
    {
        if (solver->getTime() > lastDumpTime + minDeltatDump)
        {
            lastDumpTime = solver->getTime();
            recordState();
        }

        solver->advance();
    }
    recordState();

    return {std::move(time),
            std::move(states)};
}

void exportRun(py::module &m)
{
    m.def("run_trajectory", &runTrajectory,
          "problem"_a, "solver"_a, "min_dt_dump"_a=0.0_r);
}

} // namespace pystochsim
