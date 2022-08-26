#pragma once

#include <string>
#include <tuple>
#include <vector>

namespace stochsim {

struct ParsedReactionString
{
    std::vector<std::string> reactantNames;
    std::vector<int> reactantSCs;
    std::vector<std::string> productNames;
    std::vector<int> productSCs;
    std::vector<bool> isReactantReservoir;
};

ParsedReactionString parseReactionString(std::string s);

} // namespace stochsim
