#pragma once

#include "solver.h"

#include <random>

namespace ssm {

class RLeaping: public StochasticSimulationSolver
{
public:
    RLeaping(real tend,
             real eps, real theta,
             std::vector<Reaction> reactions,
             std::vector<int> numSpecies);

    void advance() override;

private:
    int computeReactionLeap(real a0) const;

private:
    real eps_;
    real theta_;

    std::vector<real> propensities_;
    std::vector<real> cumPropensities_;

    std::vector<real> muHat_;
    std::vector<real> sigmaHatSq_;

    std::vector<int> numFirings_;
    std::vector<int> candidateNumSpecies_;

    std::vector<int> hor_; ///< highest order per species
    std::vector<int> horOrder_; ///< highest order of the reactant within the hor
    std::vector<int> isReactant_;

    std::mt19937 gen_ {42424242L};
};





} // namespace ssm
