#pragma once

#include <stochsim/reaction.h>
#include <stochsim/simulation.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <nlohmann/json.hpp>
#pragma GCC diagnostic pop

namespace stochsim {
namespace factory {

using Json = nlohmann::json;

Simulation createSimulation(const Json& j);

} // namespace factory
} // namespace stochsim
