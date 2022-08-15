#include "choose.h"

namespace ssm {
namespace utils {

size_t choose(std::span<const real> cumulativeProbs, std::mt19937& gen)
{
    const real ptot = cumulativeProbs[cumulativeProbs.size()-1];

    std::uniform_real_distribution<real> udistr(0.0_r, ptot);
    const real u = udistr(gen);

    size_t k = 0;
    while (k < cumulativeProbs.size() && u > cumulativeProbs[k])
        ++k;

    return k;
}

} // namespace utils
} // namespace ssm
