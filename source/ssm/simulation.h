#pragma once

#include <ssm/solvers/solver.h>

#include <memory>
#include <string>

namespace ssm {

class Simulation
{
public:
    Simulation(real tend,
               int numRuns,
               std::unique_ptr<StochasticSimulationSolver> method,
               std::vector<int> initialConditions,
               std::vector<std::string> speciesNames);

    void run();

private:
    real tend_;
    int numRuns_;
    std::unique_ptr<StochasticSimulationSolver> method_;
    std::vector<int> initialConditions_;
    std::vector<std::string> speciesNames_;
};

} // namespace ssm
