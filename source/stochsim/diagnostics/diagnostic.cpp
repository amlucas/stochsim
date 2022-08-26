#include "diagnostic.h"

#include <fstream>

namespace stochsim {

Diagnostic::Diagnostic() = default;
Diagnostic::~Diagnostic() = default;

void Diagnostic::dump(std::string fname)
{
    std::ofstream stream(fname);
    this->dump(stream);
}

} // namespace stochsim
