#include "R1_leaping.h"

#include <stochsim/utils/algorithms.h>
#include <numeric>

namespace stochsim {

R1Leaping::R1Leaping(real tend, int L, int sortingPeriod,
                     std::vector<Reaction> reactions,
                     std::vector<int> numSpecies)
    : StochasticSimulationSolver(tend,
                                 std::move(reactions),
                                 std::move(numSpecies))
    , L_(L)
    , sortingPeriod_(sortingPeriod)
{
    sortedReactionsIndices_.resize(reactions_.size());
    std::iota(sortedReactionsIndices_.begin(),
              sortedReactionsIndices_.end(),
              0);
}

void R1Leaping::advance()
{
    real a0 = 0.0_r;

    for (size_t k = 0; k < reactions_.size(); ++k)
    {
        const auto& r = reactions_[k];
        const real a = r.computePropensity(numSpecies_);

        a0 += a;
        propensities_[k] = a;
    }

    if (step_ % sortingPeriod_ == 0)
        utils::argsortDecreasingOrder(propensities_, sortedReactionsIndices_);

    std::gamma_distribution<real> gammaDistr(L_, 1.0_r/a0);

    const real tau = gammaDistr(gen_);
    time_ += tau;

    numFirings_.resize(reactions_.size());
    sampleNumFirings(L_, sortedReactionsIndices_, propensities_, numFirings_, gen_);

    for (size_t i = 0; i < reactions_.size(); ++i)
    {
        const int ks = numFirings_[i];
        if (ks)
            reactions_[i].applyChanges(numSpecies_, ks);
    }

    ++step_;
}

void R1Leaping::sampleNumFirings(int L,
                                 std::span<const int> sortedReactionsIndices,
                                 std::span<const real> propensities,
                                 std::span<int> numFirings,
                                 std::mt19937& gen)
{
    real a0 = 0.0_r;

    for (size_t i = 0; i < propensities.size(); ++i)
    {
        numFirings[i] = 0;
        a0 += propensities[i];
    }

    size_t m = 0;
    int s = 0;
    real cumA = 0.0_r;

    while(m < propensities.size() && s < L)
    {
        const int Im = sortedReactionsIndices[m];
        const real am = propensities[Im];

        std::binomial_distribution<int> B(L-s, am / (a0 - cumA));

        const int km = B(gen);
        numFirings[Im] = km;
        s += km;
        ++m;

        cumA += am;
    }

    if (m == propensities.size()-1)
        numFirings[sortedReactionsIndices[m]] = L - s;
}


} // namespace stochsim
