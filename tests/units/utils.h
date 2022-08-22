#pragma once

#include <gtest/gtest.h>

#define ASSERT_THROW_MESSAGE(command, exception_type, message)          \
    EXPECT_THROW({                                                      \
    try {                                                               \
        command;                                                        \
    } catch (const exception_type & e) {                                \
    ASSERT_STREQ(e.what(), message);                                    \
    throw;                                                              \
    }                                                                   \
    }, exception_type )
