#include "reaction_parser.h"

#include <optional>

namespace ssm {

static std::vector<std::string> splitStr(std::string s, std::string delim)
{
    std::vector<std::string> ret;

    auto pos = s.find(delim);
    while (pos != std::string::npos)
    {
        ret.push_back(s.substr(0, pos));
        s = s.substr(pos+delim.size());
        pos = s.find(delim);
    }
    ret.push_back(s);
    return ret;
}

static bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}


static std::tuple<std::vector<std::string>,
                  std::vector<int>>
parseSpeciesAndStoichiometricCoeffs(std::string s)
{
    std::vector<std::string> names;
    std::vector<int> SCs;
    const auto list = splitStr(std::move(s), "+");

    for (auto entry : list)
    {
        size_t i = 0;
        while (i < entry.size() && isDigit(entry[i]))
            ++i;

        const int sc = i > 0
            ? std::atoi(entry.substr(0,i).c_str())
            : 1;
        const std::string name = entry.substr(i);

        names.push_back(name);
        SCs.push_back(sc);
    }

    return {std::move(names),
            std::move(SCs)};
}


std::tuple<std::vector<std::string>,
           std::vector<int>,
           std::vector<std::string>,
           std::vector<int>>
parseReactionString(std::string s)
{
    const auto reactProds = splitStr(std::move(s), "->");

    auto [reactants, reactantsSCs] = parseSpeciesAndStoichiometricCoeffs(reactProds[0]);
    auto [products, productsSCs] = parseSpeciesAndStoichiometricCoeffs(reactProds[1]);

    return {std::move(reactants),
            std::move(reactantsSCs),
            std::move(products),
            std::move(productsSCs)};
}

} // namespace ssm
