#include <stochsim/solvers/R0_leaping.h>
#include <stochsim/solvers/R1_leaping.h>

#include <stochsim/utils/algorithms.h>

#include <gtest/gtest.h>

using namespace stochsim;

TEST( R0_sampling, numFirings_sum_up_to_L )
{
    const int L = 13;

    const std::vector<real> cumPropensities = {0.1_r, 0.5_r, 6.0_r, 7.0_r, 8.0_r};
    std::vector<int> numFirings(cumPropensities.size());

    std::mt19937 gen(23452342);

    for (int i = 0; i < 100; ++i)
    {
        R0Leaping::sampleNumFirings(L, cumPropensities, numFirings, gen);
        const int totNumFirings = std::accumulate(numFirings.begin(), numFirings.end(), 0);
        ASSERT_EQ(L, totNumFirings);
    }
}

TEST( R1_sampling, numFirings_sum_up_to_L )
{
    const int L = 13;

    const std::vector<real> propensities = {0.1_r, 0.4_r, 0.5_r, 1.0_r, 1.0_r};
    std::vector<int> numFirings(propensities.size());
    std::vector<int> sortedReactionsIndices(propensities.size());
    utils::argsortDecreasingOrder(propensities, sortedReactionsIndices);

    std::mt19937 gen(23452342);

    for (int i = 0; i < 100; ++i)
    {
        R1Leaping::sampleNumFirings(L, sortedReactionsIndices, propensities, numFirings, gen);
        const int totNumFirings = std::accumulate(numFirings.begin(), numFirings.end(), 0);
        ASSERT_EQ(L, totNumFirings);
    }
}
