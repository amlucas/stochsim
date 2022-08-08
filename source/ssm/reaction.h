#pragma once

#include "types.h"

#include <span>
#include <vector>

namespace ssm {

class Reaction
{
public:
    Reaction(real rate,
             std::vector<SpeciesId> reactantIds,
             std::vector<int> reactantSC,
             std::vector<SpeciesId> productIds,
             std::vector<int> productSC);

    real computePropensity(std::span<const int> speciesNumber) const;
    void applyChanges(std::span<int> speciesNumber) const;

private:

    real rate_;
    std::vector<SpeciesId> reactantIds_;
    std::vector<int> reactantSCs_;
    std::vector<SpeciesId> productIds_;
    std::vector<int> productSCs_;
};

} // namespace ssm
