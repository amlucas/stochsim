#include <ssm/utils/choose.h>

#include <gtest/gtest.h>

using namespace ssm;

TEST( utils, choose_one_entry_always_zero )
{
    const real cumP[] = {1.0_r};

    std::mt19937 gen(42424242);

    for (int i = 0; i < 1000; ++i)
        ASSERT_EQ(utils::choose(cumP, gen), 0);
}

TEST( utils, choose_larger_array )
{
    const real cumP[] = {0.0_r, 0.0_r, 0.0_r, 1.0_r, 1.0_r};

    std::mt19937 gen(42424242);

    for (int i = 0; i < 1000; ++i)
        ASSERT_EQ(utils::choose(cumP, gen), 3);
}
