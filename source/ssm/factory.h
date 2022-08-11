#pragma once

#include <ssm/reaction.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <nlohmann/json.hpp>
#pragma GCC diagnostic pop

namespace ssm {
namespace factory {

using json = nlohmann::json;

Reaction createReaction(const json& j);

} // namespace factory
} // namespace ssm