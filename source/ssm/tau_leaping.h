#pragma once

#include "stochastic_simulation_method.h"
#include "ssa.h"

#include <random>

namespace ssm {

class TauLeaping: public StochasticSimulationMethod
{
public:
    TauLeaping(int nc, real eps,
               std::vector<Reaction> reactions,
               std::vector<int> numSpecies);

    void advance() override;

private:
    real estimateLargestTau() const;

private:

    SSA ssa_;

    int nc_;
    real eps_;
    real acceptFactor_ {10.0_r};
    int numSSASteps_{100};

    std::vector<real> propensities_;
    std::vector<real> cumPropensities_;
    std::vector<bool> isCriticalReaction_;

    mutable std::vector<real> mu_;
    mutable std::vector<real> sigmaSquare_;

    std::vector<int> candidateNumSpecies_;
    std::vector<int> numFirings_;

    std::mt19937 gen_ {42424242L};
    std::uniform_real_distribution<real> udistr_{0.0_r, 1.0_r};
};

} // namespace ssm
