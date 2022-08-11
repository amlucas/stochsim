#include <ssm/tau_leaping.h>

#include <gtest/gtest.h>

using namespace ssm;

template <class Method>
std::vector<int> collectReaction6Gillepsie2007(const std::vector<int>& initialNumSpecies,
                                               Method& method,
                                               int numTries = 10000)
{
    std::vector<int> histogramS1(15, 0);
    for (int i = 0; i < numTries; ++i)
    {
        method.reset(initialNumSpecies);

        while (method.getTime() < 0.1_r)
            method.advance();

        const auto state = method.getState();

        if (state[0] >= 0 && state[0] < (int) histogramS1.size())
            ++histogramS1[state[0]];
    }
    return histogramS1;
}


TEST( tau_leaping, reaction6Gillepsie2007_is_same_as_SSA )
{
    /** Test if we obtain the same distribution of S1 for both tau leaping
        and SSA at time t=0.1 for the following reaction:

           c1    c2
        S1 -> S2 -> S2
     */

    const real c1 = 10.0_r;
    const real c2 = 0.1_r;

    const std::vector<int> initialNumSpecies = {9, 20000, 0};

    const int nc = 0;
    const real eps = 0.03_r;
    const real acceptFactor = 10.0_r;
    const int numStepsSSA = 1;

    const std::vector<Reaction> reactions = {Reaction(c1, {0}, {1}, {1}, {1}),
                                             Reaction(c2, {1}, {1}, {2}, {1})};

    TauLeaping tl(nc, eps, acceptFactor, numStepsSSA, reactions, initialNumSpecies);
    SSA ssa(reactions, initialNumSpecies);

    const auto histTL  = collectReaction6Gillepsie2007(initialNumSpecies, tl);
    const auto histSSA = collectReaction6Gillepsie2007(initialNumSpecies, ssa);

    const int countTL  = std::accumulate(histTL .begin(), histTL .end(), 0);
    const int countSSA = std::accumulate(histSSA.begin(), histSSA.end(), 0);

    for (size_t k = 0; k < histSSA.size(); ++k)
    {
        const real freqSSA = real(histSSA[k])/real(countSSA);
        const real freqTL  = real(histTL [k])/real(countTL );

        ASSERT_NEAR(freqSSA, freqTL, 0.01);

        //printf("%zu %g %g\n", k, freqSSA, freqTL);
    }
}
