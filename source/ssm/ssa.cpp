#include "ssa.h"

namespace ssm {

SSA::SSA(std::vector<Reaction> reactions,
         std::vector<int> numSpecies)
    : time_(0.0_r)
    , reactions_(std::move(reactions))
    , numSpecies_(std::move(numSpecies))
{}


void SSA::reset(std::vector<int> numSpecies)
{
    time_ = 0.0_r;
    numSpecies_ = std::move(numSpecies);
}

void SSA::advance()
{
    cumPropensities_.resize(reactions_.size());

    real a0 = 0.0_r;

    for (size_t k = 0; k < reactions_.size(); ++k)
    {
        const auto& r = reactions_[k];
        const real a = r.computePropensity(numSpecies_);

        a0 += a;
        cumPropensities_[k] = a0;
    }

    const real r1 = udistr_(gen_);
    const real tau = - std::log(r1) / a0;
    time_ += tau;

    const real r2 = udistr_(gen_) * a0;

    size_t k = 0;
    while (k < reactions_.size() && r2 > cumPropensities_[k])
    {
        ++k;
    }

    reactions_[k].applyChanges(numSpecies_);
}

real SSA::getTime() const
{
    return time_;
}

std::span<const int> SSA::getState() const
{
    return numSpecies_;
}


} // namespace ssm
