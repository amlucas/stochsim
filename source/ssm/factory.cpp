#include "factory.h"

#include <ssm/solvers/ssa.h>
#include <ssm/solvers/tau_leaping.h>

#include <ssm/reaction_parser.h>

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

Simulation createSimulation(const json& j)
{
    const real tend = j.at("tend").get<real>();
    const int numRuns = j.at("number of runs").get<int>();

    std::vector<int> initialSpeciesNumbers;
    std::vector<std::string> speciesNames;
    std::map<std::string, int> speciesNameToIdx;

    for (const auto& [key, value] : j.at("initial species numbers").items())
    {
        speciesNameToIdx[key] = (int) speciesNames.size();
        initialSpeciesNumbers.push_back(value);
        speciesNames.push_back(key);
    }

    std::vector<Reaction> reactions;
    for (auto reaction: j.at("reactions"))
    {
        const real rate = reaction.at("rate").get<real>();
        auto [reactants, rSCs, products, pSCs] = parseReactionString(reaction.at("reaction"));

        std::vector<int> reactantIds, productIds;

        for (auto name : reactants)
            reactantIds.push_back(speciesNameToIdx[name]);

        for (auto name : products)
            productIds.push_back(speciesNameToIdx[name]);

        reactions.emplace_back(rate,
                               std::move(reactantIds), std::move(rSCs),
                               std::move(productIds), std::move(pSCs));
    }

    const auto methodConfig = j.at("method");
    const std::string methodType = methodConfig.at("type").get<std::string>();

    std::unique_ptr<StochasticSimulationSolver> method;
    if (methodType == "SSA")
    {
        method = std::make_unique<SSA>(tend, std::move(reactions), initialSpeciesNumbers);
    }
    else if (methodType == "TauLeaping")
    {
        const real nc = methodConfig.at("nc").get<int>();
        const real eps = methodConfig.at("eps").get<real>();
        const real acceptFactor = methodConfig.at("acceptFactor").get<real>();
        const int numStepsSSA = methodConfig.at("numStepsSSA").get<int>();

        method = std::make_unique<TauLeaping>(tend, nc, eps, acceptFactor, numStepsSSA,
                                              std::move(reactions), initialSpeciesNumbers);
    }


    return {tend, numRuns,
            std::move(method),
            std::move(initialSpeciesNumbers),
            std::move(speciesNames)};
}

} // namespace factory
} // namespace ssm
