#pragma once

#include <ssm/reaction.h>

#include <map>
#include <string>
#include <vector>

namespace ssm {

class SimulationPrototype
{
public:
    SimulationPrototype(std::vector<std::string> speciesNames,
                        std::vector<int> initialSpeciesNumbers);

    void addReaction(real rate, std::string reaction);

    std::vector<Reaction> getReactions() const;

private:
    std::vector<int> initialSpeciesNumbers_;
    std::vector<std::string> speciesNames_;
    std::map<std::string, int> speciesNameToIdx_;
    std::vector<Reaction> reactions_;
};

} // namespace ssm
