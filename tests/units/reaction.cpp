#include <ssm/reaction.h>

#include <gtest/gtest.h>

using namespace ssm;

TEST( reaction, propensity_no_reactants_is_rate )
{
    const real rate = 2.345_r;

    int numSpecies[] = {10};
    const Reaction reaction(rate, {}, {}, {1}, {1});

    ASSERT_EQ(reaction.computePropensity(numSpecies),
              rate);
}
