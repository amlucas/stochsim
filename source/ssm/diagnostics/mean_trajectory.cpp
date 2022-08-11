#include "mean_trajectory.h"

namespace ssm {

MeanTrajectoryDiagnostic::MeanTrajectoryDiagnostic(std::vector<std::string> speciesNames,
                                                   real tend, int numBins)
    : Diagnostic(std::move(speciesNames))
    , dt_(tend / numBins)
    , numBins_(numBins)
{
    const size_t n = speciesNames_.size() * numBins;
    sumTrajectories_.resize(n);
    sumTrajectories_.assign(n, 0);
    counts_.resize(n);
    counts_.assign(n, 0);
}

void MeanTrajectoryDiagnostic::collect(int /* runId */, real time, std::span<const int> state)
{
    const int timeId = time / dt_;

    for (size_t i = 0; i < state.size(); ++i)
    {
        const size_t id = state.size() * timeId + i;
        sumTrajectories_[id] += state[i];
        ++counts_[id];
    }
}

void MeanTrajectoryDiagnostic::dump(std::ostream& stream)
{
    stream << "time";
    for (auto name : speciesNames_)
        stream << ',' << name;
    stream << std::endl;

    for (int timeId = 0; timeId < numBins_; ++timeId)
    {
        const real time = dt_ * timeId;
        stream << time;
        for (size_t i = 0; i < speciesNames_.size(); ++i)
        {
            const size_t id = speciesNames_.size() * timeId + i;

            const real val = counts_[id]
                ? (real) sumTrajectories_[id] / (real) counts_[id]
                : 0.0_r;

            stream << ',' << val;
        }
        stream << std::endl;
    }
}

} // namespace ssm
