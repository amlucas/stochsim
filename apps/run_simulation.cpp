#include <stochsim/factory.h>

#include <fstream>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s <json file>", argv[0]);
        exit(0);
    }

    std::ifstream jsonFile(argv[1]);
    auto config = stochsim::factory::Json::parse(jsonFile);
    auto sim = stochsim::factory::createSimulation(config);
    sim.run();

    return 0;
}
