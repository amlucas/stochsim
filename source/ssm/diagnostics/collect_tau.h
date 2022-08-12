#pragma once

#include "diagnostic.h"

#include <vector>

namespace ssm {

class CollectTauDiagnostic : public Diagnostic
{
public:
    CollectTauDiagnostic(int collectEvery);

    void collect(int runId, real time, std::span<const int> state) override;
    void dump(std::ostream& stream) override;

private:
    int collectEvery_;
    int currentRunId_ {-1};
    real previousTime_ {0.0_r};
    int numCollected_{0};

    std::vector<real> times_;
    std::vector<real> taus_;
};

} // namespace ssm
