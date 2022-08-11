#include <ssm/factory.h>

#include <gtest/gtest.h>

using namespace ssm;

TEST( factory_reaction, from_json )
{
    // reaction A + B -> 2 C
    const auto config = factory::json::parse(R"(
{
    "rate": 0.1,
    "reactants_ids": [0, 1],
    "reactants_scs": [1, 1],
    "products_ids": [2],
    "products_scs": [2]
}
)");

    const Reaction reaction = factory::createReaction(config);

    const int numSpecies[] = {4, 5, 10};
    ASSERT_EQ(reaction.computePropensity(numSpecies),
              0.1 * numSpecies[0] * numSpecies[1]);
}
