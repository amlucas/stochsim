#pragma once

#include <stochsim/types.h>

#include <random>
#include <span>

namespace stochsim {
namespace utils {

size_t choose(std::span<const real> cumulativeProbs, std::mt19937& gen);

void argsortDecreasingOrder(std::span<const real> a, std::span<int> indices);

} // namespace utils
} // namespace stochsim
