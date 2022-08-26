#include <stochsim/utils/exceptions.h>

#include "utils.h"

#include <gtest/gtest.h>

using namespace stochsim;

TEST( exceptions, construct_message )
{
    ASSERT_THROW_MESSAGE(throw BaseException("test"),
                         BaseException,
                         "test");

    ASSERT_THROW_MESSAGE(throw SyntaxError("test"),
                         SyntaxError,
                         "test");

    ASSERT_THROW_MESSAGE(throw SyntaxError("int %d and float %g and string %s", 1, 1.5f, "str"),
                         SyntaxError,
                         "int 1 and float 1.5 and string str");
}
