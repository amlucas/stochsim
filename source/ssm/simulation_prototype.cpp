#include "simulation_prototype.h"

#include <ssm/reaction_parser.h>
#include <ssm/utils/exceptions.h>

namespace ssm {

SimulationPrototype::SimulationPrototype(std::vector<std::string> speciesNames,
                                         std::vector<int> initialSpeciesNumbers)
    : initialSpeciesNumbers_(std::move(initialSpeciesNumbers))
    , speciesNames_(std::move(speciesNames))
{
    const size_t numSpecies = initialSpeciesNumbers_.size();

    if (numSpecies != speciesNames_.size())
        throw ValueError("Mismatch in size of species names and their initial number.");

    for (size_t i = 0; i < numSpecies; ++i)
    {
        const std::string name = speciesNames_[i];

        if (speciesNameToIdx_.find(name) != speciesNameToIdx_.end())
            throw ValueError("Species '%s' has been provided more than once.", name.c_str());

        speciesNameToIdx_[name] = (int) i;
    }
}

void SimulationPrototype::addReaction(real rate, std::string reactionStr)
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

std::vector<Reaction> SimulationPrototype::getReactions() const
{
    return reactions_;
}

} // namespace ssm
