#include "solver.h"

namespace stochsim {

StochasticSimulationSolver::StochasticSimulationSolver(real tend,
                                                       std::vector<Reaction> reactions,
                                                       std::vector<int> numSpecies)
    : tend_(tend)
    , time_(0.0_r)
    , step_(0)
    , reactions_(std::move(reactions))
    , numSpecies_(std::move(numSpecies))
{}

StochasticSimulationSolver::~StochasticSimulationSolver() = default;

void StochasticSimulationSolver::reset(std::vector<int> numSpecies, real time)
{
    time_ = time;
    step_ = 0;
    numSpecies_ = std::move(numSpecies);
}

real StochasticSimulationSolver::getTime() const
{
    return time_;
}

std::span<const int> StochasticSimulationSolver::getState() const
{
    return numSpecies_;
}


} // namespace stochsim
