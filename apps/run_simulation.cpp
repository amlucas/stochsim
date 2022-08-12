#include <ssm/factory.h>

#include <fstream>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s <json file>", argv[0]);
        exit(0);
    }

    std::ifstream jsonFile(argv[1]);
    auto config = ssm::factory::Json::parse(jsonFile);
    auto sim = ssm::factory::createSimulation(config);
    sim.run();

    return 0;
}
