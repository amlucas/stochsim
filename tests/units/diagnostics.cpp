#include <ssm/diagnostics/mean_trajectory.h>

#include <gtest/gtest.h>

#include <sstream>

using namespace ssm;

TEST( mean_trajectory_diagnostic, mean_one_sample_is_sample )
{
    std::vector<std::string> speciesNames = {"A"};
    const real tend = 1.0;
    const int numBins = 1;
    MeanTrajectoryDiagnostic d(speciesNames, tend, numBins);

    const int state[] = {56};
    for (int i = 0; i < 123; ++i)
        d.collect(i, tend/2, state);

    std::stringstream ss;
    d.dump(ss);

    ASSERT_EQ(ss.str(),"time,A\n0,56\n");
}
