#pragma once

#include <ssm/types.h>

#include <random>
#include <span>

namespace ssm {
namespace utils {

size_t choose(std::span<const real> cumulativeProbs, std::mt19937& gen);

} // namespace utils
} // namespace ssm
