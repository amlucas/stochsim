#include "factory.h"

#include <ssm/diagnostics/mean_trajectory.h>
#include <ssm/reaction_parser.h>
#include <ssm/solvers/ssa.h>
#include <ssm/solvers/tau_leaping.h>
#include <ssm/utils/strprintf.h>

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


static std::unique_ptr<Diagnostic> createDiagnostic(const json& j, std::vector<std::string> speciesNames)
{
    std::unique_ptr<Diagnostic> d;

    const std::string type = j.at("type").get<std::string>();

    if (type == "meanTrajectory")
    {
        const int numBins = j.at("numBins").get<int>();
        const real tend = j.at("tend").get<real>();

        d = std::make_unique<MeanTrajectoryDiagnostic>(std::move(speciesNames), tend, numBins);
    }
    else
    {
        throw std::runtime_error(utils::strprintf("Unknown diagnostic type '%s'", type.c_str()));
    }
    return d;
}


Simulation createSimulation(const json& j)
{
    const real tend = j.at("tend").get<real>();
    const int numRuns = j.at("numberOfRuns").get<int>();

    std::vector<int> initialSpeciesNumbers;
    std::vector<std::string> speciesNames;
    std::map<std::string, int> speciesNameToIdx;

    for (const auto& [key, value] : j.at("initialSpeciesNumbers").items())
    {
        speciesNameToIdx[key] = (int) speciesNames.size();
        initialSpeciesNumbers.push_back(value);
        speciesNames.push_back(key);
    }

    std::vector<Reaction> reactions;
    for (auto reaction: j.at("reactions"))
    {
        const real rate = reaction.at("rate").get<real>();
        auto [reactants, rSCs, products, pSCs, isReservoir] = parseReactionString(reaction.at("reaction"));

        std::vector<int> reactantIds, productIds;

        for (auto name : reactants)
            reactantIds.push_back(speciesNameToIdx[name]);

        for (auto name : products)
            productIds.push_back(speciesNameToIdx[name]);

        reactions.emplace_back(rate,
                               std::move(reactantIds), std::move(rSCs),
                               std::move(productIds), std::move(pSCs),
                               std::move(isReservoir));
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


    Simulation sim(tend, numRuns,
                   std::move(method),
                   std::move(initialSpeciesNumbers),
                   speciesNames);

    if (j.contains("diagnostics"))
    {
        for (auto diagConfig : j.at("diagnostics"))
        {
            const auto fname = diagConfig.at("fileName").get<std::string>();
            sim.attachDiagnostic(createDiagnostic(diagConfig, speciesNames),
                                 fname);
        }

    }

    return sim;
}

} // namespace factory
} // namespace ssm
