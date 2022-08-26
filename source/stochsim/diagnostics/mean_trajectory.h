#pragma once

#include "diagnostic.h"

#include <vector>

namespace stochsim {

class MeanTrajectoryDiagnostic : public Diagnostic
{
public:
    MeanTrajectoryDiagnostic(std::vector<std::string> speciesNames,
                             real tend, int numBins);

    void collect(int runId, real time, std::span<const int> state) override;
    void dump(std::ostream& stream) override;

private:
    real dt_;
    int numBins_;
    std::vector<std::string> speciesNames_;
    std::vector<long long int> sumTrajectories_;
    std::vector<long long int> counts_;
};

} // namespace stochsim
