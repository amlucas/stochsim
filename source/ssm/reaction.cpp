#include "reaction.h"

#include <limits>

namespace ssm {

Reaction::Reaction(real rate,
                   std::vector<SpeciesId> reactantIds,
                   std::vector<int> reactantSCs,
                   std::vector<SpeciesId> productIds,
                   std::vector<int> productSCs,
                   std::vector<bool> isReactantReservoir)
    : rate_(rate)
    , reactantIds_(std::move(reactantIds))
    , reactantSCs_(std::move(reactantSCs))
    , productIds_(std::move(productIds))
    , productSCs_(std::move(productSCs))
    , isReactantReservoir_(std::move(isReactantReservoir))
{
    if (reactantIds_.size() > 0 && isReactantReservoir_.size() == 0)
    {
        isReactantReservoir_.resize(reactantIds_.size());
        isReactantReservoir_.assign(reactantIds_.size(), false);
    }
}

real Reaction::computePropensity(std::span<const int> speciesNumber) const
{
    real propensity = rate_;

    for (size_t s = 0; s < reactantIds_.size(); ++s)
    {
        const int nu = reactantSCs_[s];
        const int x = speciesNumber[reactantIds_[s]];

        int numerator   = x;
        int denominator = nu;

        for (int k = 1; k < nu; ++k)
        {
            numerator   *= x - k;
            denominator *= k;
        }

        propensity *= numerator / denominator;
    }

    return propensity;
}

int Reaction::computeOrder() const
{
    int order = 0;
    for (auto nu : reactantSCs_)
        order += nu;
    return order;
}

int Reaction::getReactantSC(SpeciesId i) const
{
    for (size_t j = 0; j < reactantIds_.size(); ++j)
    {
        if (reactantIds_[j] == i)
            return reactantSCs_[j];
    }
    return 0;
}

real Reaction::computeGradPropensity(std::span<const int> speciesNumber,
                                     int i) const
{
    real dadxi = rate_;

    for (size_t s = 0; s < reactantIds_.size(); ++s)
    {
        const auto is = reactantIds_[s];
        const int nu = reactantSCs_[s];
        const int x = speciesNumber[is];
        real numerator {0.0_r};
        real denominator {0.0_r};

        if (i == is)
        {
            denominator = nu;

            for (int k = 0; k < nu; ++k)
            {
                int partialNumerator = 1;
                for (int j = 0; j < nu; ++j)
                {
                    if (j != k)
                        partialNumerator *= x - j;
                }
                denominator *= std::max(1, k);
                numerator += partialNumerator;
            }
        }
        else
        {
            numerator   = x;
            denominator = nu;

            for (int k = 1; k < nu; ++k)
            {
                numerator   *= x - k;
                denominator *= k;
            }
        }
        dadxi *= numerator / denominator;
    }
    return dadxi;
}


void Reaction::computeGradPropensity(std::span<const int> speciesNumber,
                                     std::span<real> dadx) const
{
    for (auto& d : dadx)
        d = 0.0_r;

    for (auto i : reactantIds_)
        dadx[i] = computeGradPropensity(speciesNumber, i);
}

real Reaction::computeF(std::span<const int> speciesNumber,
                        std::span<const int> changes) const
{
    real f = 0.0_r;

    for (int i : reactantIds_)
        f += computeGradPropensity(speciesNumber, i) * changes[i];

    return f;
}

std::tuple<real, real> Reaction::computeMuHatSigmaHatSquare(std::span<const real> propensities) const
{
    real muHat = 0.0_r;
    real sigmaHatSq = 0.0_r;

    for (size_t i = 0; i < reactantIds_.size(); ++i)
    {
        const int nu = reactantSCs_[i];
        const real a = propensities[reactantIds_[i]];
        muHat += nu * a;
        sigmaHatSq += nu * nu * a;
    }
    return {muHat, sigmaHatSq};
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

void Reaction::applyChanges(std::span<int> speciesNumber, int numFirings) const
{
    for (size_t s = 0; s < reactantIds_.size(); ++s)
    {
        if (!isReactantReservoir_[s])
            speciesNumber[reactantIds_[s]] -= numFirings * reactantSCs_[s];
    }

    for (size_t s = 0; s < productIds_.size(); ++s)
    {
        speciesNumber[productIds_[s]] += numFirings * productSCs_[s];
    }
}

std::span<const int> Reaction::getStateChange(int numSpecies) const
{
    if (stateChange_.size() == 0)
    {
        stateChange_.resize(numSpecies);
        stateChange_.assign(numSpecies, 0);
        applyChanges(stateChange_);
    }
    return stateChange_;
}

} // namespace ssm
