#include <stochsim/factory.h>
#include <stochsim/utils/exceptions.h>

#include <gtest/gtest.h>

using namespace stochsim;

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
         "numBins": 500}
    ]
}
)");

    EXPECT_NO_THROW( factory::createSimulation(config) );
}
