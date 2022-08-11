#include "simulation.h"

namespace ssm {

Simulation::Simulation(real tend,
                       int numRuns,
                       std::unique_ptr<StochasticSimulationMethod> method,
                       std::vector<int> initialConditions,
                       std::vector<std::string> speciesNames)
    : tend_(tend)
    , numRuns_(numRuns)
    , method_(std::move(method))
    , initialConditions_(std::move(initialConditions))
    , speciesNames_(std::move(speciesNames))
{}


void Simulation::run()
{
    // TODO collect diagnostics
    for (int i = 0; i < numRuns_; ++i)
    {
        method_->reset(initialConditions_);
        while (method_->getTime() < tend_)
            method_->advance();
    }
}


} // namespace ssm
