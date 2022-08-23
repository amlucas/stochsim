#pragma once

#include "solver.h"

#include <random>

namespace ssm {

class R1Leaping: public StochasticSimulationSolver
{
public:
    R1Leaping(real tend, int L, int sortingPeriod,
              std::vector<Reaction> reactions,
              std::vector<int> numSpecies);

    void advance() override;

    static void sampleNumFirings(int L,
                                 std::span<const int> sortedReactionsIndices,
                                 std::span<const real> propensities,
                                 std::span<int> numFirings,
                                 std::mt19937& gen);

private:
    int L_;
    int sortingPeriod_;

    std::vector<int> sortedReactionsIndices_;

    std::vector<real> propensities_;
    std::vector<int> numFirings_;

    std::mt19937 gen_ {42424242L};
};

} // namespace ssm
