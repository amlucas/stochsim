#pragma once

#include "strprintf.h"

#include <exception>
#include <string>

namespace ssm {

class BaseException: public std::exception
{
public:
    template<class... Args>
    BaseException(const char *fmt, Args&&... args)
        : message_(utils::strprintf(fmt, std::forward<Args>(args)...))
    {}

    const char* what() const noexcept override;

private:
    std::string message_;
};

class SyntaxError: public BaseException
{
public:
    template<class... Args>
    SyntaxError(const char *fmt, Args&&... args)
        : BaseException(fmt, std::forward<Args>(args)...)
    {}
};

class FormatError: public BaseException
{
public:
    template<class... Args>
    FormatError(const char *fmt, Args&&... args)
        : BaseException(fmt, std::forward<Args>(args)...)
    {}
};

class MissingValueError: public BaseException
{
public:
    template<class... Args>
    MissingValueError(const char *fmt, Args&&... args)
        : BaseException(fmt, std::forward<Args>(args)...)
    {}
};

class ValueError: public BaseException
{
public:
    template<class... Args>
    ValueError(const char *fmt, Args&&... args)
        : BaseException(fmt, std::forward<Args>(args)...)
    {}
};


} // namespace ssm
