#include "factory.h"

namespace ssm {
namespace factory {

Reaction createReaction(const json& j)
{
    const auto rate = j.at("rate").get<real>();
    auto reactantsIds = j.at("reactants_ids").get<std::vector<SpeciesId>>();
    auto reactantsSCs = j.at("reactants_scs").get<std::vector<int>>();
    auto productsIds = j.at("products_ids").get<std::vector<SpeciesId>>();
    auto productsSCs = j.at("products_scs").get<std::vector<int>>();

    return {rate,
            std::move(reactantsIds), std::move(reactantsSCs),
            std::move(productsIds), std::move(productsSCs)};
}

} // namespace factory
} // namespace ssm
