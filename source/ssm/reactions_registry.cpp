#include "reactions_registry.h"

#include <ssm/reaction_parser.h>
#include <ssm/utils/exceptions.h>

namespace ssm {

ReactionsRegistry::ReactionsRegistry(std::vector<std::string> speciesNames)
    : speciesNames_(std::move(speciesNames))
{
    const size_t numSpecies = speciesNames_.size();

    for (size_t i = 0; i < numSpecies; ++i)
    {
        const std::string name = speciesNames_[i];

        if (speciesNameToIdx_.find(name) != speciesNameToIdx_.end())
            throw ValueError("Species '%s' has been provided more than once.", name.c_str());

        speciesNameToIdx_[name] = (int) i;
    }
}

void ReactionsRegistry::addReaction(real rate, std::string reactionStr)
{
    auto [reactants, rSCs, products, pSCs, isReservoir] = parseReactionString(reactionStr);

    std::vector<int> reactantIds, productIds;

    for (auto name : reactants)
    {
        if (auto it = speciesNameToIdx_.find(name); it != speciesNameToIdx_.end())
        {
            reactantIds.push_back(it->second);
        }
        else
        {
            throw MissingValueError("Reactant '%s': missing initial value.", name.c_str());
        }
    }

    for (auto name : products)
    {
        if (auto it = speciesNameToIdx_.find(name); it != speciesNameToIdx_.end())
        {
            productIds.push_back(it->second);
        }
        else
        {
            throw MissingValueError("Product '%s': missing initial value.", name.c_str());
        }
    }

    reactions_.emplace_back(rate,
                            std::move(reactantIds), std::move(rSCs),
                            std::move(productIds), std::move(pSCs),
                            std::move(isReservoir));
}

std::vector<Reaction> ReactionsRegistry::getReactions() const
{
    return reactions_;
}

std::vector<std::string> ReactionsRegistry::getSpeciesNames() const
{
    return speciesNames_;
}

} // namespace ssm