#pragma once

#include <stochsim/diagnostics/diagnostic.h>
#include <stochsim/solvers/solver.h>

#include <memory>
#include <string>

namespace stochsim {

class Simulation
{
public:
    Simulation(real tend,
               int numRuns,
               std::unique_ptr<StochasticSimulationSolver> solver,
               std::vector<int> initialConditions,
               std::vector<std::string> speciesNames);

    void attachDiagnostic(std::unique_ptr<Diagnostic> diagnostic,
                          std::string fname);

    void run();

private:
    real tend_;
    int numRuns_;
    std::unique_ptr<StochasticSimulationSolver> solver_;
    std::vector<int> initialConditions_;
    std::vector<std::string> speciesNames_;

    std::vector<std::unique_ptr<Diagnostic>> diagnostics_;
    std::vector<std::string> dumpFiles_;
};

} // namespace stochsim
