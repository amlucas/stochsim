#include "tau_leaping.h"

namespace ssm {

TauLeaping::TauLeaping(int nc, real eps,
                       std::vector<Reaction> reactions,
                       std::vector<int> numSpecies)
    : StochasticSimulationMethod(std::move(reactions), std::move(numSpecies))
    , ssa_(reactions_, numSpecies_)
    , nc_(nc)
    , eps_(eps)
{}


void TauLeaping::advance()
{
    // 1. compute propensities

    propensities_.resize(reactions_.size());
    real a0 = 0.0_r;
    for (size_t k = 0; k < reactions_.size(); ++k)
    {
        const auto& r = reactions_[k];
        const real a = r.computePropensity(numSpecies_);
        propensities_[k] = a;
        a0 += a;
    }

    // 2. select critical reactions

    isCriticalReaction_.resize(reactions_.size());
    bool allReactionsAreCritical = true;

    for (size_t k = 0; k < reactions_.size(); ++k)
    {
        const real a = propensities_[k];
        const real L = reactions_[k].maximumAllowedFirings(numSpecies_);

        const bool isCritical = (a > 0) && (L <= nc_);
        isCriticalReaction_[k] = isCritical;

        if (!isCritical)
            allReactionsAreCritical = false;
    }

    // 3. estimate maximum tau

    const real tauP = allReactionsAreCritical ?
        std::numeric_limits<real>::infinity() :
        estimateLargestTau();

    // 4. Accept of reject

    if (tauP < acceptFactor_ / a0)
    {
        // reject, execute SSA.
        ssa_.reset(numSpecies_);
        for (int i = 0; i < numSSASteps_; ++i)
        {
            ssa_.advance();
        }
        time_ += ssa_.getTime();
        auto newState = ssa_.getState();
        std::copy(newState.begin(), newState.end(), numSpecies_.begin()) ;
    }
    else
    {
        // accept
        // 5. Generate taupp
        real a0c = 0;
        for (size_t i = 0; i < propensities_.size(); ++i)
        {
            if (isCriticalReaction_[i])
                a0c += propensities_[i];
        }
        const real tauPP = - std::log(udistr_(gen_)) / a0c;

        bool anySpeciesNegative = false;
        real tau = tauP < tauPP ? tauP : tauPP;
        do {
            numFirings_.resize(reactions_.size());
            numFirings_.assign(reactions_.size(), 0);

            // 6
            for (size_t i = 0; i < reactions_.size(); ++i)
            {
                if (isCriticalReaction_[i])
                {
                    numFirings_[i] = 0;
                }
                else
                {
                    std::poisson_distribution<int> pdistr(propensities_[i] * tau);
                    numFirings_[i] = pdistr(gen_);
                }
            }

            if (tauPP <= tauP) // 6.b
            {
                cumPropensities_.resize(propensities_.size());
                real cumulative = 0;
                for (size_t i = 0; i < propensities_.size(); ++i)
                {
                    if (isCriticalReaction_[i])
                        cumulative += propensities_[i];
                    cumPropensities_[i] = cumulative;
                }

                const real u = a0c * udistr_(gen_);
                size_t jc = 0;
                while (jc < reactions_.size() && u > cumPropensities_[jc])
                {
                    ++jc;
                }

                numFirings_[jc] = 1;
            }

            // 7
            candidateNumSpecies_ = numSpecies_;

            for (size_t i = 0; i < reactions_.size(); ++i)
                reactions_[i].applyChanges(candidateNumSpecies_, numFirings_[i]);

            anySpeciesNegative = false;
            for (auto x : candidateNumSpecies_)
            {
                if (x < 0)
                {
                    anySpeciesNegative = true;
                    tau /= 2;
                    break;
                }
            }
        } while (anySpeciesNegative);

        time_ += tau;
        std::swap(numSpecies_, candidateNumSpecies_);
    }
}


real TauLeaping::estimateLargestTau() const
{
    const size_t numReactions = reactions_.size();

    mu_.resize(numReactions);
    mu_.assign(numReactions, 0.0_r);
    sigmaSquare_.resize(numReactions);
    sigmaSquare_.assign(numReactions, 0.0_r);

    real a0 = 0.0_r;
    for (size_t j = 0; j < numReactions; ++j)
    {
        const auto& rj = reactions_[j];

        for (size_t jp = 0; jp < numReactions; ++jp)
        {
            if (isCriticalReaction_[jp])
                continue;

            const auto& rjp = reactions_[jp];
            const real fjjp = rj.computeF(numSpecies_, rjp.getStateChange(numSpecies_.size()));

            mu_[j] += fjjp * propensities_[jp];
            sigmaSquare_[j] += fjjp * fjjp * propensities_[jp];
        }

        a0 += propensities_[j];
    }


    real tau = std::numeric_limits<real>::max();

    for (size_t i = 0; i < numReactions; ++i)
    {
        const real muTerm    = eps_ * a0 / std::abs(mu_[i]);
        const real sigmaTerm = eps_ * eps_ * a0 * a0 / (sigmaSquare_[i] * sigmaSquare_[i]);

        tau = std::min(tau, std::min(muTerm, sigmaTerm));
    }
    return tau;
}

} // namespace ssm
