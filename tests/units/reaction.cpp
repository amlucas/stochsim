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

TEST( reaction, propensity_is_zero_for_zero_species )
{
    const real rate = 1.0_r;

    int numSpecies[] = {0};
    const Reaction reaction(rate, {0}, {1}, {}, {});

    ASSERT_EQ(reaction.computePropensity(numSpecies),
              0.0_r);
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

TEST( reaction, order )
{
    const Reaction r1(0.1, {}, {}, {0}, {1});
    ASSERT_EQ(r1.computeOrder(), 0);

    const Reaction r2(0.1, {0}, {1}, {1}, {1});
    ASSERT_EQ(r2.computeOrder(), 1);

    const Reaction r3(0.1, {0, 1}, {1, 1}, {1}, {1});
    ASSERT_EQ(r3.computeOrder(), 2);

    const Reaction r4(0.1, {0, 1}, {1, 3}, {1}, {1});
    ASSERT_EQ(r4.computeOrder(), 4);
}

TEST( reaction, maximum_allowed_firings )
{
    const real rate = 2.345_r;

    const int numSpecies[] = {10, 20, 30};
    const Reaction reaction(rate, {0, 1, 2}, {3, 1, 2}, {1}, {5});

    ASSERT_EQ(reaction.maximumAllowedFirings(numSpecies), 3);
}


TEST( reaction, grad_propensity_first_order )
{
    const real rate = 2.345_r;

    const int numSpecies[] = {10, 20, 30, 10};
    real gradients[4];
    const Reaction reaction(rate, {0, 1, 2}, {1, 1, 1}, {3}, {3});

    reaction.computeGradPropensity(numSpecies, gradients);

    ASSERT_EQ(gradients[0], rate * numSpecies[1] * numSpecies[2]);
    ASSERT_EQ(gradients[1], rate * numSpecies[0] * numSpecies[2]);
    ASSERT_EQ(gradients[2], rate * numSpecies[0] * numSpecies[1]);
    ASSERT_EQ(gradients[3], 0.0_r);
}

TEST( reaction, grad_propensity_second_order )
{
    const real rate = 2.345_r;

    const int numSpecies[] = {13, 17};
    real gradients[2];
    const Reaction reaction(rate, {0}, {2}, {1}, {1});

    reaction.computeGradPropensity(numSpecies, gradients);

    ASSERT_EQ(gradients[0], rate * (2*numSpecies[0]-1)/2.0_r);
    ASSERT_EQ(gradients[1], 0.0_r);
}

TEST( reaction, grad_propensity_third_order )
{
    const real rate = 2.345_r;

    const int numSpecies[] = {13, 17};
    real gradients[2];
    const Reaction reaction(rate, {0}, {3}, {1}, {1});

    reaction.computeGradPropensity(numSpecies, gradients);

    ASSERT_NEAR(gradients[0],
                rate * (3*numSpecies[0]*numSpecies[0]-6*numSpecies[0]+2)/6.0_r,
                1e-6_r);
    ASSERT_EQ(gradients[1], 0.0_r);
}

TEST (reaction, muHat_sigmaHatSquare )
{
    const std::vector<real> propensities = {0.1_r, 0.5_r, 0.3_r};

    {
        const Reaction r1(0.01_r, {0}, {1}, {1}, {2});
        const auto [muHat, sigmaHatSq] = r1.computeMuHatSigmaHatSquare(propensities);
        ASSERT_EQ(muHat, 1 * propensities[0]);
        ASSERT_EQ(sigmaHatSq, 1 * 1 * propensities[0]);
    }

    {
        const Reaction r1(0.01_r, {0, 2}, {1, 3}, {1}, {2});
        const auto [muHat, sigmaHatSq] = r1.computeMuHatSigmaHatSquare(propensities);
        ASSERT_EQ(muHat, 1 * propensities[0] + 3 * propensities[2]);
        ASSERT_EQ(sigmaHatSq, 1 * 1 * propensities[0] + 3 * 3 * propensities[2]);
    }
}

TEST( reaction, state_change )
{
    const real rate = 2.345_r;
    constexpr int numSpecies = 6;

    const Reaction reaction(rate, {0,2,4}, {3,1,2}, {1,3,4}, {5,2,1});

    const auto changes = reaction.getStateChange(numSpecies);

    ASSERT_EQ(changes[0], -3);
    ASSERT_EQ(changes[1], +5);
    ASSERT_EQ(changes[2], -1);
    ASSERT_EQ(changes[3], +2);
    ASSERT_EQ(changes[4], -1);
    ASSERT_EQ(changes[5],  0);
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

TEST( reaction, apply_changes_with_reservoir_variables )
{
    const real rate = 2.345_r;

    int numSpecies[] = {10, 10, 10, 10};
    const Reaction reaction(rate,
                            {0, 1, 2}, {3, 1, 2},
                            {3}, {5},
                            {true, false, false});

    reaction.applyChanges(numSpecies);

    ASSERT_EQ(numSpecies[0], 10);
    ASSERT_EQ(numSpecies[1], 9);
    ASSERT_EQ(numSpecies[2], 8);
    ASSERT_EQ(numSpecies[3], 15);
}
