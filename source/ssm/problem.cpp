#include "problem.h"

#include <ssm/reaction_parser.h>
#include <ssm/utils/exceptions.h>

namespace ssm {

template<class Key, class Val>
static std::vector<Key> getKeys(const std::map<Key, Val>& m)
{
    std::vector<Key> keys;
    for (const auto& keyval: m)
        keys.push_back(keyval.first);
    return keys;
}

template<class Key, class Val>
static std::vector<Val> getVals(const std::map<Key, Val>& m)
{
    std::vector<Val> vals;
    for (const auto& keyval: m)
        vals.push_back(keyval.second);
    return vals;
}

Problem::Problem(real tend, std::map<std::string, int> initialSpeciesNumbers)
    : Problem(tend,
              getKeys(initialSpeciesNumbers),
              getVals(initialSpeciesNumbers))
{}

Problem::Problem(real tend,
                 std::vector<std::string> speciesNames,
                 std::vector<int> initialSpeciesNumbers)
    : tend_(tend)
    , speciesNames_(std::move(speciesNames))
    , initialSpeciesNumbers_(std::move(initialSpeciesNumbers))
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

void Problem::addReaction(real rate, std::string reactionStr)
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

real Problem::getTend() const
{
    return tend_;
}

std::vector<Reaction> Problem::getReactions() const
{
    return reactions_;
}

std::vector<std::string> Problem::getSpeciesNames() const
{
    return speciesNames_;
}

std::vector<int> Problem::getInitialSpeciesNumbers() const
{
    return initialSpeciesNumbers_;
}

} // namespace ssm
