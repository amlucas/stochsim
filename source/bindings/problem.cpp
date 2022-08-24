#include "problem.h"

#include <ssm/problem.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace pyssm {

namespace py = pybind11;
using namespace pybind11::literals;

void exportProblem(py::module &m)
{
    py::class_<ssm::Problem> pyProblem(m, "Problem");

    pyProblem.def(py::init<std::map<std::string,int>>(),
                  "initial_species_numbers"_a);

    pyProblem.def("add_reaction", &ssm::Problem::addReaction,
                  "rate"_a, "reaction_str"_a);
}

} // namespace pyssm
