#pragma once

#include <stochsim/types.h>

#include <ostream>
#include <span>
#include <string>
#include <vector>

namespace stochsim {

class Diagnostic
{
public:
    Diagnostic();
    virtual ~Diagnostic();

    virtual void collect(int runId, real time, std::span<const int> state) = 0;
    virtual void dump(std::ostream& stream) = 0;

    void dump(std::string fname);
};

} // namespace stochsim
