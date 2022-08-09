#include <ssm/reaction.h>

#include <gtest/gtest.h>

using namespace ssm;

TEST( reaction, propensity_no_reactants_is_rate )
{
    const real rate = 2.345_r;

    int numSpecies[] = {10};
    const Reaction reaction(rate, {}, {}, {0}, {1});

    ASSERT_EQ(reaction.computePropensity(numSpecies),
              rate);
}

TEST( reaction, propensity_one_reactant )
{
    const real rate = 2.345_r;

    int numSpecies[] = {10};
    const Reaction reaction(rate, {0}, {1}, {}, {});

    ASSERT_EQ(reaction.computePropensity(numSpecies),
              rate * numSpecies[0]);
}

TEST( reaction, propensity_two_reactants )
{
    const real rate = 2.345_r;

    int numSpecies[] = {10, 20};
    const Reaction reaction(rate, {0, 1}, {1, 1}, {}, {});

    ASSERT_EQ(reaction.computePropensity(numSpecies),
              rate * numSpecies[0] * numSpecies[1]);
}

TEST( reaction, propensity_one_reactant_SC2 )
{
    const real rate = 2.345_r;

    int numSpecies[] = {10};
    const Reaction reaction(rate, {0}, {2}, {}, {});

    ASSERT_EQ(reaction.computePropensity(numSpecies),
              rate * numSpecies[0] * (numSpecies[0]-1) / 2);
}

TEST( reaction, propensity_one_reactant_SC3 )
{
    const real rate = 2.345_r;

    int numSpecies[] = {10};
    const Reaction reaction(rate, {0}, {3}, {}, {});

    ASSERT_EQ(reaction.computePropensity(numSpecies),
              rate * numSpecies[0] * (numSpecies[0]-1) * (numSpecies[0]-2) / 6);
}

TEST( reaction, propensity_complex )
{
    const real rate = 2.345_r;

    int numSpecies[] = {10, 20, 30};
    const Reaction reaction(rate, {0, 1, 2}, {3, 1, 2}, {1}, {5});

    ASSERT_EQ(reaction.computePropensity(numSpecies),
              rate *
              numSpecies[0] * (numSpecies[0]-1) * (numSpecies[0]-2) / 6 *
              numSpecies[1] *
              numSpecies[2] * (numSpecies[2]-1) / 2);
}


TEST( reaction, maximum_allowed_firings )
{
    const real rate = 2.345_r;

    int numSpecies[] = {10, 20, 30};
    const Reaction reaction(rate, {0, 1, 2}, {3, 1, 2}, {1}, {5});

    ASSERT_EQ(reaction.maximumAllowedFirings(numSpecies), 3);
}



TEST( reaction, apply_changes )
{
    const real rate = 2.345_r;

    int numSpecies[] = {10, 10, 10, 10};
    const Reaction reaction(rate, {0, 1, 2}, {3, 1, 2}, {3}, {5});

    reaction.applyChanges(numSpecies);

    ASSERT_EQ(numSpecies[0], 7);
    ASSERT_EQ(numSpecies[1], 9);
    ASSERT_EQ(numSpecies[2], 8);
    ASSERT_EQ(numSpecies[3], 15);
}
