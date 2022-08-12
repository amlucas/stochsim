#include "collect_tau.h"

namespace ssm {

CollectTauDiagnostic::CollectTauDiagnostic(int collectEvery)
    : Diagnostic()
    , collectEvery_(collectEvery)
{}

void CollectTauDiagnostic::collect(int runId, real time, std::span<const int> /* state */)
{
    if (runId != currentRunId_)
    {
        currentRunId_ = runId;
    }
    else
    {
        if (numCollected_ == collectEvery_)
        {
            const real tau = time - previousTime_;
            times_.push_back(previousTime_);
            taus_.push_back(tau);
        }
    }

    previousTime_ = time;
    numCollected_ += 1;

    if (numCollected_ > collectEvery_)
        numCollected_ = 0;
}

void CollectTauDiagnostic::dump(std::ostream& stream)
{
    stream << "time,tau" << std::endl;

    for (size_t i = 0; i < times_.size(); ++i)
        stream << times_[i] << ',' << taus_[i] << std::endl;
}

} // namespace ssm
