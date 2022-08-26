#include "problem.h"

#include <stochsim/problem.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace pystochsim {

namespace py = pybind11;
using namespace pybind11::literals;

void exportProblem(py::module &m)
{
    py::class_<stochsim::Problem> pyProblem(m, "Problem");

    pyProblem.def(py::init<stochsim::real, std::map<std::string,int>>(),
                  "tend"_a, "initial_species_numbers"_a);

    pyProblem.def("add_reaction", &stochsim::Problem::addReaction,
                  "rate"_a, "reaction_str"_a);
}

} // namespace pystochsim
