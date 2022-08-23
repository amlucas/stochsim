#include "exceptions.h"

namespace ssm {

const char* BaseException::what() const noexcept
{
    return message_.c_str();
}

} // namespace ssm
