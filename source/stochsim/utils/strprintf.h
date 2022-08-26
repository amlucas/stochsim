#pragma once

#include <cstdarg>
#include <string>

namespace stochsim {
namespace utils {

/** Create a std::string from a printf-style formatting.
    \param [in] fmt The c-style string that represents the format of the string, followed by extra arguments (see printf docs).
    \return The std::string corresponding to the given format evaluated with the passed extra arguments.
 */
std::string strprintf (const char *fmt, ...);

/// va_list variant of strprintf.
std::string vstrprintf(const char *fmt, va_list args);

} // namespace stochsim
} // namespace uswim
