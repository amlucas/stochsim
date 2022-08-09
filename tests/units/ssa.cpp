#include <ssm/ssa.h>

#include <gtest/gtest.h>

using namespace ssm;

TEST( ssa, decay )
{
    const real rate = 1.0_r;
    const real tend = 0.1_r;
    const int n0 = 10000;
    const int numRuns = 1000;

    SSA ssa({Reaction{rate, {0}, {1}, {}, {}}},
            {n0});

    real meanEnd = 0.0_r;
    for (int i = 0; i < numRuns; ++i)
    {
        ssa.reset({n0});

        while (ssa.getTime() < tend)
        {
            ssa.advance();
        }

        meanEnd += ssa.getState()[0];
    }
    meanEnd /= numRuns;

    ASSERT_NEAR(meanEnd,
                n0 * std::exp(-tend * rate),
                1.0_r);
}

TEST( ssa, equilibrium )
{
    const real rate = 0.5_r;
    const real tend = 10.0_r;
    const int n0 = 1000;
    const int numRuns = 1000;

    SSA ssa({Reaction{rate, {0}, {1}, {1}, {1}}, // A -> B
             Reaction{rate, {1}, {1}, {0}, {1}}}, // B -> A
            {n0, 0});

    real meanEnd = 0.0_r;
    for (int i = 0; i < numRuns; ++i)
    {
        ssa.reset({n0, 0});

        while (ssa.getTime() < tend)
        {
            ssa.advance();
        }

        meanEnd += ssa.getState()[0];
    }
    meanEnd /= numRuns;

    ASSERT_NEAR(meanEnd,
                n0 * 0.5_r,
                1.0_r);
}
