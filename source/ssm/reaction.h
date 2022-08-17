#pragma once

#include "types.h"

#include <span>
#include <tuple>
#include <vector>

namespace ssm {

class Reaction
{
public:
    Reaction(real rate,
             std::vector<SpeciesId> reactantIds,
             std::vector<int> reactantSC,
             std::vector<SpeciesId> productIds,
             std::vector<int> productSC,
             std::vector<bool> isReactantReservoir = {});

    real computePropensity(std::span<const int> speciesNumber) const;

    int computeOrder() const;

    int getReactantSC(SpeciesId i) const;

    real computeGradPropensity(std::span<const int> speciesNumber, int i) const;
    void computeGradPropensity(std::span<const int> speciesNumber, std::span<real> dadx) const;
    real computeF(std::span<const int> speciesNumber, std::span<const int> changes) const;

    void addContributionMuHatSigmaHatSquare(real propensity,
                                            std::span<real> muHat,
                                            std::span<real> sigmaHatSq) const;

    int maximumAllowedFirings(std::span<const int> speciesNumber) const;

    void applyChanges(std::span<int> speciesNumber, int numFirings=1) const;

    std::span<const int> getStateChange(int numSpecies) const;

private:

    real rate_;
    std::vector<SpeciesId> reactantIds_;
    std::vector<int> reactantSCs_;
    std::vector<SpeciesId> productIds_;
    std::vector<int> productSCs_;
    std::vector<bool> isReactantReservoir_;

    mutable std::vector<int> stateChange_;
};

} // namespace ssm
