#pragma once

#include <stochsim/reaction.h>

#include <map>
#include <string>
#include <vector>

namespace stochsim {

class Problem
{
public:
    Problem(real tend,
            std::map<std::string, int> initialSpeciesNumbers);

    Problem(real tend,
            std::vector<std::string> speciesNames,
            std::vector<int> initialSpeciesNumbers);

    void addReaction(real rate, std::string reactionStr);

    real getTend() const;
    std::vector<Reaction> getReactions() const;
    std::vector<std::string> getSpeciesNames() const;
    std::vector<int> getInitialSpeciesNumbers() const;

private:
    real tend_;
    std::vector<std::string> speciesNames_;
    std::vector<int> initialSpeciesNumbers_;
    std::map<std::string, int> speciesNameToIdx_;
    std::vector<Reaction> reactions_;
};

} // namespace stochsim
