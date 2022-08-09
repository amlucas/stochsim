#include "stochastic_simulation_method.h"

namespace ssm {

StochasticSimulationMethod::StochasticSimulationMethod(std::vector<Reaction> reactions,
                                                       std::vector<int> numSpecies)
    : time_(0.0_r)
    , reactions_(std::move(reactions))
    , numSpecies_(std::move(numSpecies))
{}


void StochasticSimulationMethod::reset(std::vector<int> numSpecies)
{
    time_ = 0.0_r;
    numSpecies_ = std::move(numSpecies);
}

real StochasticSimulationMethod::getTime() const
{
    return time_;
}

std::span<const int> StochasticSimulationMethod::getState() const
{
    return numSpecies_;
}


} // namespace ssm
