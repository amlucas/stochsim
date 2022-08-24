#pragma once

#include <ssm/reaction.h>

#include <map>
#include <string>
#include <vector>

namespace ssm {

class ReactionsRegistry
{
public:
    ReactionsRegistry(std::vector<std::string> speciesNames);

    void addReaction(real rate, std::string reactionStr);

    std::vector<Reaction> getReactions() const;
    std::vector<std::string> getSpeciesNames() const;

private:
    std::vector<std::string> speciesNames_;
    std::map<std::string, int> speciesNameToIdx_;
    std::vector<Reaction> reactions_;
};

} // namespace ssm
