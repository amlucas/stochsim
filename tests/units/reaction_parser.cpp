#include <ssm/reaction_parser.h>

#include <gtest/gtest.h>

using namespace ssm;

TEST( reaction_parser, fails_on_wrong_format )
{
    EXPECT_THROW( parseReactionString("A+B->3C->D"), std::runtime_error );
    EXPECT_THROW( parseReactionString("A + 3 -> C"), std::runtime_error );
    EXPECT_THROW( parseReactionString("A + B -> C + "), std::runtime_error );
    EXPECT_THROW( parseReactionString("A -> "), std::runtime_error );
    EXPECT_THROW( parseReactionString(" -> B"), std::runtime_error );
}

TEST( reaction_parser, parse_no_spaces )
{
    auto [reactants, reactantsSCs, products, productsSCs] = parseReactionString("A+B->3C");

    ASSERT_EQ(reactants.size(), 2);
    ASSERT_EQ(products.size(), 1);
    ASSERT_EQ(reactants[0], "A");
    ASSERT_EQ(reactants[1], "B");
    ASSERT_EQ(products[0], "C");
    ASSERT_EQ(reactantsSCs[0], 1);
    ASSERT_EQ(reactantsSCs[1], 1);
    ASSERT_EQ(productsSCs[0], 3);
}

TEST( reaction_parser, supports_spaces )
{
    auto [reactants, reactantsSCs, products, productsSCs] = parseReactionString("A + B -> 3 C");

    ASSERT_EQ(reactants.size(), 2);
    ASSERT_EQ(products.size(), 1);
    ASSERT_EQ(reactants[0], "A");
    ASSERT_EQ(reactants[1], "B");
    ASSERT_EQ(products[0], "C");
    ASSERT_EQ(reactantsSCs[0], 1);
    ASSERT_EQ(reactantsSCs[1], 1);
    ASSERT_EQ(productsSCs[0], 3);
}

TEST( reaction_parser, complex )
{
    auto [reactants, reactantsSCs, products, productsSCs] = parseReactionString("A + 45 B + 7C-> 3 C +67D");

    ASSERT_EQ(reactants.size(), 3);
    ASSERT_EQ(products.size(), 2);
    ASSERT_EQ(reactants[0], "A");
    ASSERT_EQ(reactants[1], "B");
    ASSERT_EQ(reactants[2], "C");
    ASSERT_EQ(products[0], "C");
    ASSERT_EQ(products[1], "D");
    ASSERT_EQ(reactantsSCs[0], 1);
    ASSERT_EQ(reactantsSCs[1], 45);
    ASSERT_EQ(reactantsSCs[2], 7);
    ASSERT_EQ(productsSCs[0], 3);
    ASSERT_EQ(productsSCs[1], 67);
}
