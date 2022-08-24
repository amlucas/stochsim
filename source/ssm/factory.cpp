#include "factory.h"

#include <ssm/diagnostics/collect_tau.h>
#include <ssm/diagnostics/mean_trajectory.h>
#include <ssm/reaction_parser.h>
#include <ssm/simulation_prototype.h>
#include <ssm/solvers/R0_leaping.h>
#include <ssm/solvers/R1_leaping.h>
#include <ssm/solvers/R_leaping.h>
#include <ssm/solvers/ssa.h>
#include <ssm/solvers/tau_leaping.h>
#include <ssm/utils/exceptions.h>

namespace ssm {
namespace factory {

Reaction createReaction(const Json& j)
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


static std::unique_ptr<Diagnostic> createDiagnostic(const Json& j, std::vector<std::string> speciesNames)
{
    std::unique_ptr<Diagnostic> d;

    const std::string type = j.at("type").get<std::string>();

    if (type == "meanTrajectory")
    {
        const int numBins = j.at("numBins").get<int>();
        const real tend = j.at("tend").get<real>();

        d = std::make_unique<MeanTrajectoryDiagnostic>(std::move(speciesNames), tend, numBins);
    }
    else if (type == "collectTau")
    {
        const int collectEvery = j.at("collectEvery").get<int>();

        d = std::make_unique<CollectTauDiagnostic>(collectEvery);
    }
    else
    {
        throw SyntaxError("Unknown diagnostic type '%s'", type.c_str());
    }
    return d;
}

static std::unique_ptr<StochasticSimulationSolver> createSolver(const Json& j, real tend,
                                                                std::vector<Reaction> reactions,
                                                                std::vector<int> initialSpeciesNumbers)
{
    const auto solverConfig = j.at("solver");
    const std::string solverType = solverConfig.at("type").get<std::string>();

    std::unique_ptr<StochasticSimulationSolver> solver;

    if (solverType == "SSA")
    {
        solver = std::make_unique<SSA>(tend,
                                       std::move(reactions),
                                       std::move(initialSpeciesNumbers));
    }
    else if (solverType == "TauLeaping")
    {
        const real nc = solverConfig.at("nc").get<int>();
        const real eps = solverConfig.at("eps").get<real>();
        const real acceptFactor = solverConfig.at("acceptFactor").get<real>();
        const int numStepsSSA = solverConfig.at("numStepsSSA").get<int>();

        solver = std::make_unique<TauLeaping>(tend, nc, eps, acceptFactor, numStepsSSA,
                                              std::move(reactions),
                                              std::move(initialSpeciesNumbers));
    }
    else if (solverType == "R0Leaping")
    {
        const int L = solverConfig.at("L").get<int>();

        solver = std::make_unique<R0Leaping>(tend, L,
                                             std::move(reactions),
                                             std::move(initialSpeciesNumbers));
    }
    else if (solverType == "R1Leaping")
    {
        const int L = solverConfig.at("L").get<int>();
        const int sortingPeriod = solverConfig.at("sortingPeriod").get<int>();

        solver = std::make_unique<R1Leaping>(tend, L, sortingPeriod,
                                             std::move(reactions),
                                             std::move(initialSpeciesNumbers));
    }
    else if (solverType == "RLeaping")
    {
        const real eps = solverConfig.at("eps").get<real>();
        const real theta = solverConfig.at("theta").get<real>();
        const int sortingPeriod = solverConfig.at("sortingPeriod").get<int>();

        solver = std::make_unique<RLeaping>(tend, eps, theta, sortingPeriod,
                                            std::move(reactions),
                                            std::move(initialSpeciesNumbers));
    }
    else
    {
        throw SyntaxError("Unknown solver type '%s'", solverType.c_str());
    }
    return solver;
}

Simulation createSimulation(const Json& j)
{
    const real tend = j.at("tend").get<real>();
    const int numRuns = j.at("numberOfRuns").get<int>();

    std::vector<int> initialSpeciesNumbers;
    std::vector<std::string> speciesNames;

    for (const auto& [key, value] : j.at("initialSpeciesNumbers").items())
    {
        initialSpeciesNumbers.push_back(value);
        speciesNames.push_back(key);
    }

    SimulationPrototype simulationPrototype(speciesNames);

    for (auto reaction: j.at("reactions"))
    {
        const real rate = reaction.at("rate").get<real>();
        const std::string reactionStr = reaction.at("reaction").get<std::string>();
        simulationPrototype.addReaction(rate, reactionStr);
    }

    auto solver = createSolver(j, tend, simulationPrototype.getReactions(), initialSpeciesNumbers);

    Simulation sim(tend, numRuns,
                   std::move(solver),
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
