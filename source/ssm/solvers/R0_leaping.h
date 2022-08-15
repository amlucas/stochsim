#pragma once

#include "solver.h"

#include <random>

namespace ssm {

class R0Leaping: public StochasticSimulationSolver
{
public:
    R0Leaping(real tend, int L,
              std::vector<Reaction> reactions,
              std::vector<int> numSpecies);

    void advance() override;

private:
    int L_;

    std::vector<real> cumPropensities_;
    std::vector<int> numFirings_;

    std::mt19937 gen_ {42424242L};
};

} // namespace ssm
