#include "ssa.h"

namespace ssm {

SSA::SSA(real tend,
         std::vector<Reaction> reactions,
         std::vector<int> numSpecies)
    : StochasticSimulationSolver(tend,
                                 std::move(reactions),
                                 std::move(numSpecies))
{}


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

    real tau = - std::log(r1) / a0;
    if (time_ + tau > tend_)
        tau = tend_ - time_;

    time_ += tau;

    if (a0 == 0)
        return;

    const real r2 = udistr_(gen_) * a0;

    size_t k = 0;
    while (k < reactions_.size() && r2 > cumPropensities_[k])
    {
        ++k;
    }

    reactions_[k].applyChanges(numSpecies_);
}


} // namespace ssm
