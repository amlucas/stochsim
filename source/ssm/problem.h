#pragma once

#include <ssm/reaction.h>

#include <map>
#include <string>
#include <vector>

namespace ssm {

class Problem
{
public:
    Problem(std::map<std::string, int> initialSpeciesNumbers);

    Problem(std::vector<std::string> speciesNames,
            std::vector<int> initialSpeciesNumbers);

    void addReaction(real rate, std::string reactionStr);

    std::vector<Reaction> getReactions() const;
    std::vector<std::string> getSpeciesNames() const;
    std::vector<int> getInitialSpeciesNumbers() const;

private:
    std::vector<std::string> speciesNames_;
    std::vector<int> initialSpeciesNumbers_;
    std::map<std::string, int> speciesNameToIdx_;
    std::vector<Reaction> reactions_;
};

} // namespace ssm
