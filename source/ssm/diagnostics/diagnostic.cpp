#include "diagnostic.h"

#include <fstream>

namespace ssm {

Diagnostic::Diagnostic(std::vector<std::string> speciesNames)
    :speciesNames_(std::move(speciesNames))
{}

Diagnostic::~Diagnostic() = default;

void Diagnostic::dump(std::string fname)
{
    std::ofstream stream(fname);
    this->dump(stream);
}

} // namespace ssm
