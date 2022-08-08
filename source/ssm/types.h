#pragma once

namespace ssm {

using real = double;

using SpeciesId = int;

inline namespace literals {
constexpr real operator "" _r(long double val)
{
    return static_cast<real>(val);
}
} // inline namespace literals

} // namespace ssm
