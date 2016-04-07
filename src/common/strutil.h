#pragma once
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <utility>
#include <errno.h>

class StrUtil
{
public:
    inline static void to_str(double val, std::string* str)
    {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "%f", val);
        *str = buffer;
    }

    inline static std::string to_str(double val)
    {
        std::string str;
        to_str(val, &str);
        return str;
    }

    inline static void to_str(int32_t val, std::string* str)
    {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "%d", val);
        *str = buffer;
    }

    inline static std::string to_str(int32_t val)
    {
        std::string str;
        to_str(val, &str);
        return str;
    }

    inline static void to_str(uint32_t val, std::string* str)
    {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "%u", val);
        *str = buffer;
    }

    inline static std::string to_str(uint32_t val)
    {
        std::string str;
        to_str(val, &str);
        return str;
    }

    inline static void to_str(int64_t val, std::string* str)
    {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "%ld", val);
        *str = buffer;
    }

    inline static std::string to_str(int64_t val)
    {
        std::string str;
        to_str(val, &str);
        return str;
    }

    inline static void to_str(uint64_t val, std::string* str)
    {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "%lu", val);
        *str = buffer;
    }

    inline static std::string to_str(uint64_t val)
    {
        std::string str;
        to_str(val, &str);
        return str;
    }

    inline static void to_str(uint32_t val, int leading_zeros, std::string* str)
    {
        char buffer[256];
        std::string format = std::string("%0") + to_str(leading_zeros) + "u";
        snprintf(buffer, sizeof(buffer), format.c_str(), val);
        *str = buffer;
    }

    inline static std::string to_str(uint32_t val, int leading_zeros)
    {
        std::string str;
        to_str(val, leading_zeros, &str);
        return str;
    }

    inline static std::string to_str(bool val)
    {
        if      (val == true)
        {
            return "true";
        }
        else
        {
            return "false";
        }
    }

    inline static std::string to_str(const std::string& val)
    {
        return val;
    }

    inline static std::string to_lower(const std::string& str)
    {
        std::string result(str);
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }

    inline static bool to_bool(const std::string& str)
    {
        if (str == "true")
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    inline static bool from_str(const std::string& str, int16_t* result)
    {
        char* end_ptr;
        const char* str_ptr = str.c_str();
        errno = 0;
        *result = (int32_t)strtol(str_ptr, &end_ptr, 10);
        if (end_ptr == str_ptr || *end_ptr != '\0' ||
            ((*result == LONG_MIN || *result == LONG_MAX) && errno == ERANGE))
        {
            return false;
        }

        return true;
    }

    inline static bool from_str(const std::string& str, uint16_t* result)
    {
        char* end_ptr;
        const char* str_ptr = str.c_str();
        errno = 0;
        *result = (uint32_t)strtoul(str_ptr, &end_ptr, 10);
        if (end_ptr == str_ptr || *end_ptr != '\0' ||
            ((*result == LONG_MIN || *result == LONG_MAX) && errno == ERANGE))
        {
            return false;
        }

        return true;
    }

    inline static bool from_str(const std::string& str, int32_t* result)
    {
        char* end_ptr;
        const char* str_ptr = str.c_str();
        errno = 0;
        *result = (int32_t)strtol(str_ptr, &end_ptr, 10);
        if (end_ptr == str_ptr || *end_ptr != '\0' ||
            ((*result == LONG_MIN || *result == LONG_MAX) && errno == ERANGE))
        {
            return false;
        }

        return true;
    }

    inline static bool from_str(const std::string& str, uint32_t* result)
    {
        char* end_ptr;
        const char* str_ptr = str.c_str();
        errno = 0;
        *result = (uint32_t)strtoul(str_ptr, &end_ptr, 10);
        if (end_ptr == str_ptr || *end_ptr != '\0' ||
            ((*result == LONG_MIN || *result == LONG_MAX) && errno == ERANGE))
        {
            return false;
        }

        return true;
    }

    inline static bool from_str(const std::string& str, int64_t* result)
    {
        char* end_ptr;
        const char* str_ptr = str.c_str();
        errno = 0;
        *result = strtol(str_ptr, &end_ptr, 10);
        if (end_ptr == str_ptr || *end_ptr != '\0' ||
            ((*result == LONG_MIN || *result == LONG_MAX) && errno == ERANGE))
        {
            return false;
        }

        return true;
    }

    inline static bool from_str(const std::string& str, uint64_t* result)
    {
        char* end_ptr;
        const char* str_ptr = str.c_str();
        errno = 0;
        *result = strtoul(str_ptr, &end_ptr, 10);
        if (end_ptr == str_ptr || *end_ptr != '\0' ||
            ((*result == LONG_MIN || *result == LONG_MAX) && errno == ERANGE))
        {
            return false;
        }

        return true;
    }

    inline static bool from_str(const std::string& str, float* result)
    {
        char* end_ptr;
        const char* str_ptr = str.c_str();
        errno = 0;
        *result = strtof(str_ptr, &end_ptr);
        if (end_ptr == str_ptr || *end_ptr != '\0' || errno == ERANGE)
        {
            return false;
        }

        return true;
    }

    inline static bool from_str(const std::string& str, double* result)
    {
        char* end_ptr;
        const char* str_ptr = str.c_str();
        errno = 0;
        *result = strtod(str_ptr, &end_ptr);
        if (end_ptr == str_ptr || *end_ptr != '\0' || errno == ERANGE)
        {
            return false;
        }

        return true;
    }

    static std::vector<std::string> &split_str(const std::string &s, char delim, std::vector<std::string> &elems) {
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            elems.push_back(item);
        }
        return elems;
    }


    static std::vector<std::string> split_str(const std::string &s, char delim) {
        std::vector<std::string> elems;
        split_str(s, delim, elems);
        return elems;
    }
};
