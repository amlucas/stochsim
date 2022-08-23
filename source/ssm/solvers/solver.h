#pragma once

#include <ssm/reaction.h>

#include <span>
#include <vector>

namespace ssm {

class StochasticSimulationSolver
{
public:
    StochasticSimulationSolver(real tend,
                               std::vector<Reaction> reactions,
                               std::vector<int> numSpecies);

    virtual ~StochasticSimulationSolver();

    virtual void reset(std::vector<int> numSpecies, real time = 0.0_r);
    virtual void advance() = 0;

    real getTime() const;
    int getStep() const;

    std::span<const int> getState() const;

protected:
    real tend_;
    real time_;
    int step_;
    std::vector<Reaction> reactions_;
    std::vector<int> numSpecies_;
};

} // namespace ssm
