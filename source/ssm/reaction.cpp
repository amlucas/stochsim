#include "reaction.h"

#include <limits>

namespace ssm {

Reaction::Reaction(real rate,
                   std::vector<SpeciesId> reactantIds,
                   std::vector<int> reactantSCs,
                   std::vector<SpeciesId> productIds,
                   std::vector<int> productSCs)
    : rate_(rate)
    , reactantIds_(std::move(reactantIds))
    , reactantSCs_(std::move(reactantSCs))
    , productIds_(std::move(productIds))
    , productSCs_(std::move(productSCs))

{}

real Reaction::computePropensity(std::span<const int> speciesNumber) const
{
    real propensity = rate_;

    for (size_t s = 0; s < reactantIds_.size(); ++s)
    {
        int nu = reactantSCs_[s];
        const int x = speciesNumber[reactantIds_[s]];

        int numerator   = x;
        int denominator = nu;

        while((--nu) > 0)
        {
            numerator   *= x - nu;
            denominator *= nu;
        }

        propensity *= numerator / denominator;
    }

    return propensity;
}

int Reaction::maximumAllowedFirings(std::span<const int> speciesNumber) const
{
    int L = std::numeric_limits<int>::max();

    for (size_t s = 0; s < reactantIds_.size(); ++s)
    {
        const int x  = speciesNumber[reactantIds_[s]];
        const int nu = reactantSCs_[s];

        if (nu > 0)
            L = std::min(L, x/nu);
    }

    return L;
}

void Reaction::applyChanges(std::span<int> speciesNumber) const
{
    for (size_t s = 0; s < reactantIds_.size(); ++s)
    {
        speciesNumber[reactantIds_[s]] -= reactantSCs_[s];
    }

    for (size_t s = 0; s < productIds_.size(); ++s)
    {
        speciesNumber[productIds_[s]] += productSCs_[s];
    }
}

} // namespace ssm
