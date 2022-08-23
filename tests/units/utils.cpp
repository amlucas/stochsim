#include <ssm/utils/algorithms.h>

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

TEST( utils, argsort )
{
    constexpr int n = 6;
    const real a[n] = {3.0_r, 4.0_r, 1.0_r, 2.0_r, 6.0_r, 5.0_r};
    const int refIndices[n] = {2, 3, 0, 1, 5, 4};

    int indices[n] = {0};
    utils::argsort(a, indices);

    for (int i = 0; i < n; ++i)
        ASSERT_EQ(indices[i], refIndices[i]);
}
