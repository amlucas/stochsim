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

TEST( mean_trajectory_diagnostic, multiple_bins_and_species )
{
    std::vector<std::string> speciesNames = {"A", "B"};
    const real tend = 1.0;
    const int numBins = 4;
    MeanTrajectoryDiagnostic d(speciesNames, tend, numBins);

    std::vector<int> state;

    state = {2, 3};
    d.collect(0, 0.1_r, state);
    d.collect(0, 0.3_r, state);

    state = {4, 5};
    d.collect(0, 0.1_r, state);

    std::stringstream ss;
    d.dump(ss);

    ASSERT_EQ(ss.str(),"time,A,B\n0,3,4\n0.25,2,3\n0.5,0,0\n0.75,0,0\n");
}
