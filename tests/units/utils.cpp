#include <stochsim/utils/algorithms.h>

#include <gtest/gtest.h>

using namespace stochsim;

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

TEST( utils, argsortDecreasingOrder )
{
    constexpr int n = 6;
    const real a[n] = {3.0_r, 4.0_r, 1.0_r, 2.0_r, 6.0_r, 5.0_r};
    const int refIndices[n] = {4, 5, 1, 0, 3, 2};

    int indices[n] = {0};
    utils::argsortDecreasingOrder(a, indices);

    for (int i = 0; i < n; ++i)
        ASSERT_EQ(indices[i], refIndices[i]);
}
