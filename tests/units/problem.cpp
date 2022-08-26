#include <stochsim/problem.h>
#include "utils.h"

#include <stochsim/utils/exceptions.h>

#include <gtest/gtest.h>

using namespace stochsim;

TEST( problem, throw_errors_for_wrong_initial_species )
{
    ASSERT_THROW_MESSAGE(Problem p(10.0_r, {{"A", -5}}),
                         ValueError,
                         "Species 'A' has a wrong initial value, must be non-negative (got -5).");

    ASSERT_THROW_MESSAGE(Problem p(10.0_r, {"A", "B", "A"}, {5, 0, 2}),
                         ValueError,
                         "Species 'A' has been provided more than once.");
}

TEST( problem, throw_errors_for_missing_initial_species )
{
    ASSERT_THROW_MESSAGE({
            Problem p(10.0_r, {{"B", 5}});
            p.addReaction(0.1_r, "A->B");
        },
        MissingValueError,
        "Reactant 'A': missing initial value.");


    ASSERT_THROW_MESSAGE({
            Problem p(10.0_r, {{"A", 5}, {"C",0}});
            p.addReaction(0.1_r, "A->B+C");
        },
        MissingValueError,
        "Product 'B': missing initial value.");
}
