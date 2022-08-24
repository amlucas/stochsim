#include "reactions_registry.h"

#include <ssm/reactions_registry.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace pyssm {

namespace py = pybind11;
using namespace pybind11::literals;

void exportReactionsRegistry(py::module &m)
{
    py::class_<ssm::ReactionsRegistry> pyReactionsReg(m, "ReactionsRegistry");

    pyReactionsReg.def(py::init<std::vector<std::string>>(),
                       "species_names"_a);

    pyReactionsReg.def("add_reaction", &ssm::ReactionsRegistry::addReaction,
                       "rate"_a, "reaction_str"_a);
}

} // namespace pyssm
