#include "R0_leaping.h"

#include <ssm/utils/choose.h>

namespace ssm {

R0Leaping::R0Leaping(real tend, int L,
                     std::vector<Reaction> reactions,
                     std::vector<int> numSpecies)
    : StochasticSimulationSolver(tend,
                                 std::move(reactions),
                                 std::move(numSpecies))
    , L_(L)
{}

void R0Leaping::advance()
{
    real a0 = 0.0_r;

    for (size_t k = 0; k < reactions_.size(); ++k)
    {
        const auto& r = reactions_[k];
        const real a = r.computePropensity(numSpecies_);

        a0 += a;
        cumPropensities_[k] = a0;
    }

    std::gamma_distribution<real> gammaDistr(L_, 1.0_r/a0);

    const real tau = gammaDistr(gen_);

    numFirings_.resize(reactions_.size());
    numFirings_.assign(reactions_.size(), 0);

    for (int i = 0; i < L_; ++i)
    {
        const size_t k = utils::choose(cumPropensities_, gen_);
        ++numFirings_[k];
    }

    time_ += tau;

    for (size_t i = 0; i < reactions_.size(); ++i)
        reactions_[i].applyChanges(numSpecies_, numFirings_[i]);
}

} // namespace ssm
