#include "R_leaping.h"
#include "R0_leaping.h"
#include "R1_leaping.h"

#include <stochsim/utils/algorithms.h>

#include <numeric>

namespace stochsim {

static void computeHor(std::span<const Reaction> reactions,
                       std::span<int> hor,
                       std::span<int> isReactant,
                       std::span<int> horOrder)
{
    for (size_t i = 0; i < hor.size(); ++i)
    {
        int ho = 0;
        int id = -1;
        int maxSpeciesOrder = -1;

        for (size_t j = 0; j < reactions.size(); ++j)
        {
            const auto& r = reactions[j];
            const int nu = r.getReactantSC(i);

            if (nu == 0) // the species is not part of this reaction.
                continue;

            const int order = r.computeOrder();
            if (order >= ho)
            {
                ho = order;
                id = j;
                if (order > ho || maxSpeciesOrder < nu)
                    maxSpeciesOrder = nu;
            }
        }

        hor[i] = ho;
        isReactant[i] = id >= 0;
        horOrder[i] = maxSpeciesOrder;
    }
}

RLeaping::RLeaping(real tend,
                   real eps, real theta, int sortingPeriod,
                   std::vector<Reaction> reactions,
                   std::vector<int> numSpecies)
    : StochasticSimulationSolver(tend,
                                 std::move(reactions),
                                 std::move(numSpecies))
    , eps_(eps)
    , theta_(theta)
    , sortingPeriod_(sortingPeriod)
{
    sortedReactionsIndices_.resize(reactions_.size(), 0);
    std::iota(sortedReactionsIndices_.begin(),
              sortedReactionsIndices_.end(),
              0);

    propensities_   .resize(reactions_.size());
    cumPropensities_.resize(reactions_.size());
    numFirings_     .resize(reactions_.size());

    muHat_     .resize(numSpecies_.size());
    sigmaHatSq_.resize(numSpecies_.size());

    isReactant_.resize(numSpecies_.size());
    hor_       .resize(numSpecies_.size());
    horOrder_  .resize(numSpecies_.size());
    computeHor(reactions_, hor_, isReactant_, horOrder_);
}

static real computePropensities(std::span<const Reaction> reactions,
                                std::span<const int> numSpecies,
                                std::span<real> propensities,
                                std::span<real> cumPropensities)
{
    real a0 = 0.0_r;
    for (size_t i = 0; i < reactions.size(); ++i)
    {
        const real a = reactions[i].computePropensity(numSpecies);
        a0 += a;
        propensities[i] = a;
        cumPropensities[i] = a0;
    }
    return a0;
}

static void computeMuHatSigmaHatSq(std::span<const Reaction> reactions,
                                   std::span<const real> propensities,
                                   std::span<real> muHat,
                                   std::span<real> sigmaHatSq)
{
    for (auto& val : muHat)
        val = 0.0_r;

    for (auto& val : sigmaHatSq)
        val = 0.0_r;

    for (size_t i = 0; i < reactions.size(); ++i)
    {
        const auto& r = reactions[i];
        const real a = propensities[i];
        r.addContributionMuHatSigmaHatSquare(a, muHat, sigmaHatSq);
    }
}

static int computeLpp(real theta, real a0,
                      std::span<const Reaction> reactions,
                      std::span<const int> numSpecies,
                      std::span<const real> propensities)
{
    int Lpp = std::numeric_limits<int>::max();
    for (size_t j = 0; j < reactions.size(); ++j)
    {
        const int Lj = reactions[j].maximumAllowedFirings(numSpecies);
        const real aj = propensities[j];
        if (aj == 0.0_r)
            continue;
        const int proposed = (1.0_r - theta * (1.0_r - a0/aj)) * Lj;
        Lpp = std::min(Lpp, proposed);
    }
    return Lpp;
}

void RLeaping::advance()
{
    const real a0 = computePropensities(reactions_, numSpecies_,
                                        propensities_, cumPropensities_);

    if (step_ % sortingPeriod_ == 0)
        utils::argsortDecreasingOrder(propensities_, sortedReactionsIndices_);

    computeMuHatSigmaHatSq(reactions_, propensities_,
                           muHat_, sigmaHatSq_);

    const int Lp = computeReactionLeap(a0);
    const int Lpp = computeLpp(theta_, a0, reactions_, numSpecies_, propensities_);

    int L = std::min(Lp, Lpp);
    bool allSpeciesPositiveAfterChange = false;

    while (!allSpeciesPositiveAfterChange)
    {
        if (L < (int) reactions_.size())
        {
            R0Leaping::sampleNumFirings(L, cumPropensities_, numFirings_, gen_);
        }
        else
        {
            R1Leaping::sampleNumFirings(L, sortedReactionsIndices_,
                                        propensities_, numFirings_, gen_);
        }

        candidateNumSpecies_ = numSpecies_;

        for (size_t i = 0; i < reactions_.size(); ++i)
        {
            const int ks = numFirings_[i];
            if (ks)
                reactions_[i].applyChanges(candidateNumSpecies_, numFirings_[i]);
        }

        allSpeciesPositiveAfterChange = true;
        for (auto x : candidateNumSpecies_)
        {
            if (x < 0)
            {
                L /= 2;
                allSpeciesPositiveAfterChange = false;
                break;
            }
        }
    }

    std::swap(numSpecies_, candidateNumSpecies_);

    std::gamma_distribution<real> gammaDistr(L, 1.0_r/a0);
    const real tau = gammaDistr(gen_);
    time_ += tau;
    ++step_;
}

int RLeaping::computeReactionLeap(real a0) const
{
    real L = std::numeric_limits<int>::max();
    for (size_t i = 0; i < numSpecies_.size(); ++i)
    {
        if (!isReactant_[i])
            continue;

        const int x = numSpecies_[i];

        real g = hor_[i];
        switch(hor_[i])
        {
        case 1:
            g = 1.0_r;
            break;

        case 2:
            g = 2.0_r;
            if (horOrder_[i] > 1)
                g += 1.0_r / (x - 1.0_r);
            break;

        case 3:
            g = 3.0_r;
            if (horOrder_[i] == 2)
                g = 1.5_r * (2.0_r + 1.0_r / (x - 1.0_r));
            else if (horOrder_[i] == 3)
                g = 3.0_r + 1.0_r / (x - 1.0_r) + 2.0_r / (x - 2.0_r);
            break;

        default:
            break;
        }

        const real muHat = muHat_[i];
        const real sigmaHatSq = sigmaHatSq_[i];

        const real increment = std::max(1.0_r, eps_ * x / g);
        const real muTerm = increment / std::abs(muHat);
        const real sigmaTerm = increment * increment / std::abs(sigmaHatSq - muHat*muHat/a0);

        const real Lcandidate = a0 * std::min(muTerm, sigmaTerm);

        L = std::min(L+1e-6_r, Lcandidate);
    }
    return (int) L;
}

} // namespace stochsim
