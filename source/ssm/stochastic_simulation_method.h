#pragma once

#include "reaction.h"

#include <span>
#include <vector>

namespace ssm {

class StochasticSimulationMethod
{
public:
    StochasticSimulationMethod(std::vector<Reaction> reactions,
                               std::vector<int> numSpecies);

    void reset(std::vector<int> numSpecies);
    virtual void advance() = 0;

    real getTime() const;
    std::span<const int> getState() const;

protected:
    real time_;
    std::vector<Reaction> reactions_;
    std::vector<int> numSpecies_;
};

} // namespace ssm
