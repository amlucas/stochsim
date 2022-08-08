#pragma once

#include "reaction.h"

#include <random>
#include <span>
#include <vector>

namespace ssm {

class SSA
{
public:
    SSA(std::vector<Reaction> reactions,
        std::vector<int> numSpecies);

    void reset(std::vector<int> numSpecies);
    void advance();

    real getTime() const;
    std::span<const int> getState() const;

private:
    real time_;

    std::vector<Reaction> reactions_;
    std::vector<int> numSpecies_;

    std::vector<real> propensities_;
    std::mt19937 gen_ {42424242L};
    std::uniform_real_distribution<real> udistr_{0.0_r, 1.0_r};
};

} // namespace ssm
