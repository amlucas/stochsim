#pragma once

#include "stochastic_simulation_method.h"

#include <random>

namespace ssm {

class SSA: public StochasticSimulationMethod
{
public:
    SSA(std::vector<Reaction> reactions,
        std::vector<int> numSpecies);

    void advance() override;

private:
    std::vector<real> cumPropensities_;
    std::mt19937 gen_ {42424242L};
    std::uniform_real_distribution<real> udistr_{0.0_r, 1.0_r};
};

} // namespace ssm
