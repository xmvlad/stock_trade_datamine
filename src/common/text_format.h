#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <string>


inline std::string text_format(const char* fmt, ...)
{
    char str_buffer[4096];
    va_list arglist;
    va_start(arglist, fmt);
    vsnprintf(str_buffer, 4096, fmt, arglist);
    va_end(arglist);
    return std::string(str_buffer);
}

