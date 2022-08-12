#include "reaction_parser.h"
#include <ssm/utils/strprintf.h>

#include <optional>

namespace ssm {

static inline std::vector<std::string> splitStr(std::string s, std::string delim)
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

static inline bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

static inline std::string trimSpaces(std::string s)
{
    // trim left spaces
    {
        size_t i = 0;
        while (i < s.size() && s[i] == ' ')
            ++i;

        s = s.substr(i);
    }
    // trim right spaces
    {
        int i = (int) s.size()-1;
        while (i >= 0 && s[i] == ' ')
            --i;
        s = s.substr(0, i+1);
    }
    return s;
}

static std::tuple<std::vector<std::string>,
                  std::vector<int>>
parseSpeciesAndStoichiometricCoeffs(std::string s)
{
    std::vector<std::string> names;
    std::vector<int> SCs;
    const auto list = splitStr(std::move(s), "+");

    // special case of spaces only / empty string: return no reaction.
    if (list.size() == 1 && trimSpaces(list[0]).size() == 0)
        return {{}, {}};

    for (auto entry : list)
    {
        entry = trimSpaces(entry);

        size_t i = 0;
        while (i < entry.size() && isDigit(entry[i]))
            ++i;

        const int sc = i > 0
            ? std::atoi(entry.substr(0,i).c_str())
            : 1;
        const std::string name = trimSpaces(entry.substr(i));

        names.push_back(name);
        SCs.push_back(sc);
    }

    return {std::move(names),
            std::move(SCs)};
}

static inline bool isReservoir(std::string name)
{
    name = trimSpaces(name);
    return
        name.size() > 2 &&
        *(name.begin()) == '[' &&
        *(name.end()-1) == ']';
}

static inline bool containsSpaces(std::string name)
{
    for (auto c : name)
    {
        if (c == ' ')
            return true;
    }
    return false;
}

std::tuple<std::vector<std::string>,
           std::vector<int>,
           std::vector<std::string>,
           std::vector<int>,
           std::vector<bool>>
parseReactionString(std::string s)
{
    const auto reactProds = splitStr(s, "->");

    if (reactProds.size() != 2)
    {
        throw std::runtime_error(utils::strprintf("Wrong reaction format: expect exactly one '->', got '%s'", s.c_str()));
    }

    auto [reactants, reactantsSCs] = parseSpeciesAndStoichiometricCoeffs(reactProds[0]);
    auto [products, productsSCs] = parseSpeciesAndStoichiometricCoeffs(reactProds[1]);

    for (auto r : reactants)
    {
        if (r.size() == 0)
            throw std::runtime_error("Reactant name is empty.");
        if (containsSpaces(r))
            throw std::runtime_error(utils::strprintf("Invalid reactant name '%s': must not contain spaces.", r.c_str()));
    }
    for (auto r : products)
    {
        if (r.size() == 0)
            throw std::runtime_error("Product name is empty.");
        if (containsSpaces(r))
            throw std::runtime_error(utils::strprintf("Invalid product name '%s': must not contain spaces.", r.c_str()));
    }

    std::vector<bool> isReactantReservoir;
    for (const auto& name : reactants)
        isReactantReservoir.push_back(isReservoir(name));

    return {std::move(reactants),
            std::move(reactantsSCs),
            std::move(products),
            std::move(productsSCs),
            std::move(isReactantReservoir)};
}

} // namespace ssm
