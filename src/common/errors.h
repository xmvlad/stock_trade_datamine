#pragma once

#include <exception>
#include <stdexcept>
#include <stdio.h>
#include "text_format.h"


class RuntimeError: public std::runtime_error
{
public:
    RuntimeError(const std::string& what): std::runtime_error(what) {}
};

class FileError: public RuntimeError
{
public:
    FileError(const std::string& what): RuntimeError(what) {}
};

class ParseError: public RuntimeError
{
public:
    ParseError(const std::string& what): RuntimeError(what) {}
};


inline std::string format_exception(const std::string& what, const int line, const char* file, const char* function)
{
    return text_format("%s at %s, %s, %d", what.c_str(), file, function, line);
}

#define THROW(TYPE, FMT...) throw TYPE(format_exception(text_format(FMT), __LINE__, __FILE__, __PRETTY_FUNCTION__));

// TODO fixme change to assert
#define DESTRUCTOR_ERROR(WHAT)  do { printf("Destructor error: %s", WHAT); std::terminate(); } while (0);

