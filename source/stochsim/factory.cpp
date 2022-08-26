#include "factory.h"

#include <stochsim/diagnostics/collect_tau.h>
#include <stochsim/diagnostics/mean_trajectory.h>
#include <stochsim/problem.h>
#include <stochsim/solvers/R0_leaping.h>
#include <stochsim/solvers/R1_leaping.h>
#include <stochsim/solvers/R_leaping.h>
#include <stochsim/solvers/ssa.h>
#include <stochsim/solvers/tau_leaping.h>
#include <stochsim/utils/exceptions.h>

namespace stochsim {
namespace factory {

static std::unique_ptr<Problem> createProblem(const Json& j)
{
    const real tend = j.at("tend").get<real>();
    std::vector<int> initialSpeciesNumbers;
    std::vector<std::string> speciesNames;

    for (const auto& [key, value] : j.at("initialSpeciesNumbers").items())
    {
        initialSpeciesNumbers.push_back(value);
        speciesNames.push_back(key);
    }

    auto problem = std::make_unique<Problem>(tend,
                                             std::move(speciesNames),
                                             std::move(initialSpeciesNumbers));

    for (auto reaction: j.at("reactions"))
    {
        const real rate = reaction.at("rate").get<real>();
        const std::string reactionStr = reaction.at("reaction").get<std::string>();
        problem->addReaction(rate, reactionStr);
    }

    return problem;
}

static std::unique_ptr<StochasticSimulationSolver> createSolver(const Json& j, const Problem *p)
{
    const auto solverConfig = j.at("solver");
    const std::string solverType = solverConfig.at("type").get<std::string>();

    std::unique_ptr<StochasticSimulationSolver> solver;

    if (solverType == "SSA")
    {
        solver = std::make_unique<SSA>(p->getTend(),
                                       p->getReactions(),
                                       p->getInitialSpeciesNumbers());
    }
    else if (solverType == "TauLeaping")
    {
        const real nc = solverConfig.at("nc").get<int>();
        const real eps = solverConfig.at("eps").get<real>();
        const real acceptFactor = solverConfig.at("acceptFactor").get<real>();
        const int numStepsSSA = solverConfig.at("numStepsSSA").get<int>();

        solver = std::make_unique<TauLeaping>(p->getTend(),
                                              nc, eps, acceptFactor, numStepsSSA,
                                              p->getReactions(),
                                              p->getInitialSpeciesNumbers());
    }
    else if (solverType == "R0Leaping")
    {
        const int L = solverConfig.at("L").get<int>();

        solver = std::make_unique<R0Leaping>(p->getTend(),
                                             L,
                                             p->getReactions(),
                                             p->getInitialSpeciesNumbers());
    }
    else if (solverType == "R1Leaping")
    {
        const int L = solverConfig.at("L").get<int>();
        const int sortingPeriod = solverConfig.at("sortingPeriod").get<int>();

        solver = std::make_unique<R1Leaping>(p->getTend(),
                                             L, sortingPeriod,
                                             p->getReactions(),
                                             p->getInitialSpeciesNumbers());
    }
    else if (solverType == "RLeaping")
    {
        const real eps = solverConfig.at("eps").get<real>();
        const real theta = solverConfig.at("theta").get<real>();
        const int sortingPeriod = solverConfig.at("sortingPeriod").get<int>();

        solver = std::make_unique<RLeaping>(p->getTend(),
                                            eps, theta, sortingPeriod,
                                            p->getReactions(),
                                            p->getInitialSpeciesNumbers());
    }
    else
    {
        throw SyntaxError("Unknown solver type '%s'", solverType.c_str());
    }
    return solver;
}

static std::unique_ptr<Diagnostic> createDiagnostic(const Json& j, const Problem *p)
{
    std::unique_ptr<Diagnostic> d;

    const std::string type = j.at("type").get<std::string>();

    if (type == "meanTrajectory")
    {
        const int numBins = j.at("numBins").get<int>();

        d = std::make_unique<MeanTrajectoryDiagnostic>(p->getSpeciesNames(), p->getTend(), numBins);
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


Simulation createSimulation(const Json& j)
{
    const int numRuns = j.at("numberOfRuns").get<int>();

    auto problem = createProblem(j);
    auto solver = createSolver(j, problem.get());

    Simulation sim(problem->getTend(),
                   numRuns,
                   std::move(solver),
                   problem->getInitialSpeciesNumbers(),
                   problem->getSpeciesNames());

    if (j.contains("diagnostics"))
    {
        for (auto diagConfig : j.at("diagnostics"))
        {
            const auto fname = diagConfig.at("fileName").get<std::string>();
            sim.attachDiagnostic(createDiagnostic(diagConfig, problem.get()),
                                 fname);
        }
    }

    return sim;
}

} // namespace factory
} // namespace stochsim
