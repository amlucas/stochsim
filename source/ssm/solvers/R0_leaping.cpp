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
    time_ += tau;

    numFirings_.resize(reactions_.size());
    sampleNumFirings(L_, cumPropensities_, numFirings_, gen_);

    for (size_t i = 0; i < reactions_.size(); ++i)
        reactions_[i].applyChanges(numSpecies_, numFirings_[i]);
}

void R0Leaping::sampleNumFirings(int L,
                                 std::span<const real> cumPropensities,
                                 std::span<int> numFirings,
                                 std::mt19937& gen)
{
    for (auto& n : numFirings)
        n = 0;

    for (int i = 0; i < L; ++i)
    {
        const size_t k = utils::choose(cumPropensities, gen);
        ++numFirings[k];
    }
}


} // namespace ssm
