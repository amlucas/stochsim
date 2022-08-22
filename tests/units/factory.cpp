#include <ssm/factory.h>

#include <gtest/gtest.h>

using namespace ssm;

TEST( factory_reaction, from_json )
{
    // reaction A + B -> 2 C
    const auto config = factory::Json::parse(R"(
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


TEST( factory_simulation, from_json_ssa )
{
    const auto config = factory::Json::parse(R"(
{
    "solver" : {
        "type": "SSA"
    },
    "initialSpeciesNumbers": {
        "S1": 9,
        "S2": 20000,
        "S3": 0
    },
    "reactions": [
        {"rate": 10.0, "reaction": "S1->S2"},
        {"rate": 0.1, "reaction": "S2->S3"}
    ],
    "tend": 0.1,
    "numberOfRuns": 10000
}
)");

    EXPECT_NO_THROW( factory::createSimulation(config) );
}

TEST( factory_simulation, missing_initial_reactant_value )
{
    const auto config = factory::Json::parse(R"(
{
    "solver" : {
        "type": "SSA"
    },
    "initialSpeciesNumbers": {
        "B": 10
    },
    "reactions": [
        {"rate": 1.0, "reaction": "A->B"}
    ],
    "tend": 0.1,
    "numberOfRuns": 1
}
)");

    EXPECT_THROW({
            try
            {
                factory::createSimulation(config);
            }
            catch (const std::runtime_error& e)
            {
                ASSERT_STREQ(e.what(), "Reactant 'A': missing initial value.");
                throw;
            }
        }, std::runtime_error );
}


TEST( factory_simulation, missing_initial_product_value )
{
    const auto config = factory::Json::parse(R"(
{
    "solver" : {
        "type": "SSA"
    },
    "initialSpeciesNumbers": {
        "A": 10
    },
    "reactions": [
        {"rate": 1.0, "reaction": "A->B"}
    ],
    "tend": 0.1,
    "numberOfRuns": 1
}
)");

    EXPECT_THROW({
            try
            {
                factory::createSimulation(config);
            }
            catch (const std::runtime_error& e)
            {
                ASSERT_STREQ(e.what(), "Product 'B': missing initial value.");
                throw;
            }
        }, std::runtime_error );
}


TEST( factory_simulation, from_json_tau_leaping )
{
    const auto config = factory::Json::parse(R"(
{
    "solver" : {
        "type": "TauLeaping",
        "nc": 10,
        "acceptFactor": 10,
        "eps": 0.03,
        "numStepsSSA": 100
    },
    "initialSpeciesNumbers": {
        "S1": 9,
        "S2": 20000,
        "S3": 0
    },
    "reactions": [
        {"rate": 10.0, "reaction": "S1->S2"},
        {"rate": 0.1, "reaction": "S2->S3"}
    ],
    "tend": 0.1,
    "numberOfRuns": 10000
}
)");

    EXPECT_NO_THROW( factory::createSimulation(config) );
}

TEST( factory_simulation, with_diagnostic )
{
    const auto config = factory::Json::parse(R"(
{
    "solver" : {
        "type": "TauLeaping",
        "nc": 10,
        "acceptFactor": 10,
        "eps": 0.03,
        "numStepsSSA": 100
    },
    "initialSpeciesNumbers": {
        "S1": 9,
        "S2": 20000,
        "S3": 0
    },
    "reactions": [
        {"rate": 10.0, "reaction": "S1->S2"},
        {"rate": 0.1, "reaction": "S2->S3"}
    ],
    "tend": 100,
    "numberOfRuns": 10,
    "diagnostics" : [
        {"type": "meanTrajectory",
         "fileName": "test.csv",
         "tend": 100,
         "numBins": 500}
    ]
}
)");

    EXPECT_NO_THROW( factory::createSimulation(config) );
}
