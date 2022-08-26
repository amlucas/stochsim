#include "simulation.h"

namespace stochsim {

Simulation::Simulation(real tend,
                       int numRuns,
                       std::unique_ptr<StochasticSimulationSolver> solver,
                       std::vector<int> initialConditions,
                       std::vector<std::string> speciesNames)
    : tend_(tend)
    , numRuns_(numRuns)
    , solver_(std::move(solver))
    , initialConditions_(std::move(initialConditions))
    , speciesNames_(std::move(speciesNames))
{}

void Simulation::attachDiagnostic(std::unique_ptr<Diagnostic> diagnostic,
                                  std::string fname)
{
    diagnostics_.push_back(std::move(diagnostic));
    dumpFiles_.push_back(fname);
}

void Simulation::run()
{
    for (int i = 0; i < numRuns_; ++i)
    {
        solver_->reset(initialConditions_);
        while (solver_->getTime() < tend_)
        {
            for (auto& d : diagnostics_)
                d->collect(i, solver_->getTime(), solver_->getState());

            solver_->advance();
        }
    }

    for (size_t i = 0; i < diagnostics_.size(); ++i)
        diagnostics_[i]->dump(dumpFiles_[i]);
}


} // namespace stochsim
