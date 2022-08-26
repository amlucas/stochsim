#include "collect_tau.h"

namespace stochsim {

CollectTauDiagnostic::CollectTauDiagnostic(int collectEvery)
    : Diagnostic()
    , collectEvery_(collectEvery)
{}

void CollectTauDiagnostic::collect(int runId, real time, std::span<const int> /* state */)
{
    if (runId != currentRunId_)
    {
        currentRunId_ = runId;
        numCollected_ = 0;
    }
    else
    {
        if (numCollected_ % collectEvery_ == 0)
        {
            const real tau = time - previousTime_;
            times_.push_back(previousTime_);
            taus_.push_back(tau);
        }
        ++numCollected_;
    }

    previousTime_ = time;
}

void CollectTauDiagnostic::dump(std::ostream& stream)
{
    stream << "time,tau" << std::endl;

    for (size_t i = 0; i < times_.size(); ++i)
        stream << times_[i] << ',' << taus_[i] << std::endl;
}

} // namespace stochsim
