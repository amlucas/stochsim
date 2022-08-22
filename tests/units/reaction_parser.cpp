#include <ssm/reaction_parser.h>

#include <gtest/gtest.h>

using namespace ssm;

#define ASSERT_THROW_MESSAGE(command, exception_type, message)          \
    EXPECT_THROW({                                                      \
    try {                                                               \
        command;                                                        \
    } catch (const exception_type & e) {                                \
    ASSERT_STREQ(e.what(), message);                                    \
    throw;                                                              \
    }                                                                   \
    }, exception_type )


TEST( reaction_parser, fails_on_wrong_format )
{
    ASSERT_THROW_MESSAGE(parseReactionString("A+B->3C->D"),
                         std::runtime_error,
                         "Wrong reaction format: expect exactly one '->', got 'A+B->3C->D'");

    ASSERT_THROW_MESSAGE(parseReactionString("A + 3 -> C"),
                         std::runtime_error,
                         "Reactant name is empty.");

    ASSERT_THROW_MESSAGE(parseReactionString("A + B -> C + "),
                         std::runtime_error,
                         "Product name is empty.");

    ASSERT_THROW_MESSAGE(parseReactionString("A B -> C"),
                         std::runtime_error,
                         "Invalid reactant name 'A B': must not contain spaces.");

    ASSERT_THROW_MESSAGE(parseReactionString("A -> C D"),
                         std::runtime_error,
                         "Invalid product name 'C D': must not contain spaces.");
}

TEST( reaction_parser, parse_no_spaces )
{
    auto [reactants, reactantsSCs, products, productsSCs, isReactantReservoir] = parseReactionString("A+B->3C");

    ASSERT_EQ(reactants.size(), 2);
    ASSERT_EQ(products.size(), 1);
    ASSERT_EQ(reactants[0], "A");
    ASSERT_EQ(reactants[1], "B");
    ASSERT_EQ(products[0], "C");
    ASSERT_EQ(reactantsSCs[0], 1);
    ASSERT_EQ(reactantsSCs[1], 1);
    ASSERT_EQ(productsSCs[0], 3);

    ASSERT_FALSE(isReactantReservoir[0]);
    ASSERT_FALSE(isReactantReservoir[1]);
}

TEST( reaction_parser, supports_spaces )
{
    auto [reactants, reactantsSCs, products, productsSCs, isReactantReservoir] = parseReactionString("A + B -> 3 C");

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
    auto [reactants, reactantsSCs, products, productsSCs, isReactantReservoir] = parseReactionString("A + 45 B + 7C-> 3 C +67D");

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

TEST( reaction_parser, source_is_valid_reactions )
{
    auto [r, rSCs, p, pSCs, _] = parseReactionString("A ->");

    ASSERT_EQ(r.size(), 1);
    ASSERT_EQ(p.size(), 0);
    ASSERT_EQ(rSCs[0], 1);
    ASSERT_EQ(r[0], "A");
}

TEST( reaction_parser, sink_is_valid_reactions )
{
    auto [r, rSCs, p, pSCs, _] = parseReactionString(" -> B");

    ASSERT_EQ(r.size(), 0);
    ASSERT_EQ(p.size(), 1);
    ASSERT_EQ(pSCs[0], 1);
    ASSERT_EQ(p[0], "B");
}

TEST( reaction_parser, reservoir_variable )
{
    auto [r, rSCs, p, pSCs, isReactantReservoir] = parseReactionString("[A] -> B");

    ASSERT_EQ(r.size(), 1);
    ASSERT_EQ(p.size(), 1);
    ASSERT_EQ(rSCs[0], 1);
    ASSERT_EQ(pSCs[0], 1);
    ASSERT_EQ(r[0], "[A]");
    ASSERT_EQ(p[0], "B");

    ASSERT_TRUE(isReactantReservoir[0]);
}
