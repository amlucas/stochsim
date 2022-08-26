#include <stochsim/solvers/tau_leaping.h>

#include <gtest/gtest.h>

using namespace stochsim;

template <class Method>
std::vector<int> collectReaction6Gillepsie2007(real tend,
                                               const std::vector<int>& initialNumSpecies,
                                               Method& method,
                                               int numTries = 10000)
{
    std::vector<int> histogramS1(initialNumSpecies[0]+1, 0);
    for (int i = 0; i < numTries; ++i)
    {
        method.reset(initialNumSpecies);

        while (method.getTime() < tend)
            method.advance();

        const auto state = method.getState();
        const int S1 = state[0];

        if (S1 >= 0 && S1 < (int) histogramS1.size())
            ++histogramS1[S1];
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

    const real tend = 0.1_r;
    const real c1 = 10.0_r;
    const real c2 = 0.1_r;

    const std::vector<int> initialNumSpecies = {9, 20000, 0};

    const int nc = 10;
    const real eps = 0.03_r;
    const real acceptFactor = 10.0_r;
    const int numStepsSSA = 100;

    const std::vector<Reaction> reactions = {Reaction(c1, {0}, {1}, {1}, {1}),
                                             Reaction(c2, {1}, {1}, {2}, {1})};

    TauLeaping tl(tend, nc, eps, acceptFactor, numStepsSSA, reactions, initialNumSpecies);
    SSA ssa(tend, reactions, initialNumSpecies);

    const auto histTL  = collectReaction6Gillepsie2007(tend, initialNumSpecies, tl);
    const auto histSSA = collectReaction6Gillepsie2007(tend, initialNumSpecies, ssa);

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
