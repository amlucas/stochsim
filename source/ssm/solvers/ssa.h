#pragma once

#include "solver.h"

#include <random>

namespace ssm {

class SSA: public StochasticSimulationSolver
{
public:
    SSA(real tend,
        std::vector<Reaction> reactions,
        std::vector<int> numSpecies);

    void advance() override;

private:
    std::vector<real> cumPropensities_;
    std::mt19937 gen_ {42424242L};
    std::uniform_real_distribution<real> udistr_{0.0_r, 1.0_r};
};

} // namespace ssm
