/****************************************************************************
 **
 ** This file is part of the Ponder library, formerly CAMP.
 **
 ** The MIT License (MIT)
 **
 ** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Copyright (C) 2015-2016 Billy Quith.
  **
 ** Permission is hereby granted, free of charge, to any person obtaining a copy
 ** of this software and associated documentation files (the "Software"), to deal
 ** in the Software without restriction, including without limitation the rights
 ** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 ** copies of the Software, and to permit persons to whom the Software is
 ** furnished to do so, subject to the following conditions:
 **
 ** The above copyright notice and this permission notice shall be included in
 ** all copies or substantial portions of the Software.
 **
 ** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 ** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 ** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 ** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 ** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 ** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 ** THE SOFTWARE.
 **
 ****************************************************************************/


#include <ponder/detail/util.hpp>

#if !defined(WIN32)
#   include <strings.h>
#endif

// Convert to string:
//  #include "format.h"
//  http://zverovich.net/2013/09/07/integer-to-string-conversion-in-cplusplus.html

// Convert from string:
//  http://tinodidriksen.com/2010/02/16/cpp-convert-string-to-int-speed/

namespace ponder
{
namespace detail
{

static inline int stricmp(const char* a, const char* b)
{
#ifdef WIN32
	return _strcmpi(a, b);
#else
	return strcasecmp(a, b);
#endif
}

// parse string
    
template <typename T>
static bool parse_integer(IdRef from, T& to)
{
    try {
        const long p = std::stol(from.c_str());
        to = static_cast<T>(p);
    } catch (std::logic_error&) {
        return false;
    }
    return true;
}

bool conv(IdRef from, char& to)
{
    if (from.length() == 1)
    {
        to = from[0];
        return true;
    }
    return false;
}

bool conv(IdRef from, unsigned char& to)
{
    char r;
    if (!conv(from,r))
        return false;
    to = r;
    return true;
}

bool conv(IdRef from, short& to)
{
    return parse_integer(from, to);
}

bool conv(IdRef from, unsigned short& to)
{
    return parse_integer(from, to);
}

bool conv(IdRef from, int& to)
{
    return parse_integer(from, to);
}

bool conv(IdRef from, unsigned int& to)
{
    return parse_integer(from, to);
}

bool conv(IdRef from, long& to)
{
    return parse_integer(from, to);
}

bool conv(IdRef from, unsigned long& to)
{
    return parse_integer(from, to);
}

bool conv(IdRef from, long long& to)
{
    try {
        to = std::stoll(from.c_str());
    } catch (std::logic_error&) {
        return false;
    }
    return true;
}

bool conv(IdRef from, unsigned long long& to)
{
    try {
        to = std::stoull(from.c_str());
    } catch (std::logic_error&) {
        return false;
    }
    return true;
}
    
bool conv(IdRef from, bool& to)
{
    const char *s = from.c_str();
    if (stricmp(s, "1")==0 || stricmp(s, "true")==0)
    {
        to = true;
        return true;
    }
    else if (stricmp(s, "0")==0 || stricmp(s, "false")==0)
    {
        to = false;
        return true;
    }
    return false;
}

bool conv(IdRef from, float& to)
{
    try {
        to = std::stof(from.c_str());
    } catch (std::logic_error&) {
        return false;
    }
    return true;
}

bool conv(IdRef from, double& to)
{
    try {
        to = std::stod(from.c_str());
    } catch (std::logic_error&) {
        return false;
    }
    return true;
}


static const char* c_typeNames[] =
{
    "none",     // ValueType::None
    "bool",     // ValueType::Boolean
    "int",      // ValueType::Integer,
    "real",     // ValueType::Real,
    "string",   // ValueType::String,
    "enum",     // ValueType::Enum,
    "array",    // ValueType::Array,
    "user",     // ValueType::User
};

const char* typeAsString(ValueType t)
{
    const unsigned int i = static_cast<unsigned int>(t);
    return i <= static_cast<unsigned int>(ValueType::User) ? c_typeNames[i] : "unknown";
}

} // namespace detail
} // namespace ponder
