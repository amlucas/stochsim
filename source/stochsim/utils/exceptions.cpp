#include "exceptions.h"

namespace stochsim {

const char* BaseException::what() const noexcept
{
    return message_.c_str();
}

} // namespace stochsim
