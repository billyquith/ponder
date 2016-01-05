/****************************************************************************
 **
 ** This file is part of the CAMP library.
 **
 ** The MIT License (MIT)
 **
 ** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
 ** Contact: Tegesoft Information (contact@tegesoft.com)
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

// Convert to string:
//  #include "format.h"
//  http://zverovich.net/2013/09/07/integer-to-string-conversion-in-cplusplus.html

// Convert from string:
//  http://tinodidriksen.com/2010/02/16/cpp-convert-string-to-int-speed/

namespace ponder
{
namespace detail
{

//-------------------------------------------------------------------------------------------------
// parse string
    
template <typename T>
static bool parse_integer(const std::string& from, T& to)
{
    try {
        const long p = std::stol(from.c_str());
        to = static_cast<T>(p);
    } catch (std::logic_error&) {
        return false;
    }
    return true;
}

bool conv(const std::string& from, char& to)
{
    if (from.length() == 1)
    {
        to = from[0];
        return true;
    }
    return false;
}

bool conv(const std::string& from, unsigned char& to)
{
    char r;
    if (!conv(from,r))
        return false;
    to = r;
    return true;
}

bool conv(const std::string& from, short& to)
{
    return parse_integer(from, to);
}

bool conv(const std::string& from, unsigned short& to)
{
    return parse_integer(from, to);
}

bool conv(const std::string& from, int& to)
{
    return parse_integer(from, to);
}

bool conv(const std::string& from, unsigned int& to)
{
    return parse_integer(from, to);
}

bool conv(const std::string& from, long& to)
{
    return parse_integer(from, to);
}

bool conv(const std::string& from, unsigned long& to)
{
    return parse_integer(from, to);
}

bool conv(const std::string& from, long long& to)
{
    try {
        to = std::stoll(from.c_str());
    } catch (std::logic_error&) {
        return false;
    }
    return true;
}

bool conv(const std::string& from, unsigned long long& to)
{
    try {
        to = std::stoull(from.c_str());
    } catch (std::logic_error&) {
        return false;
    }
    return true;
}
    
bool conv(const std::string& from, bool& to)
{
    const char *s = from.c_str();
    if (strcasecmp(s, "1")==0 || strcasecmp(s, "true")==0)
    {
        to = true;
        return true;
    }
    else if (strcasecmp(s, "0")==0 || strcasecmp(s, "false")==0)
    {
        to = false;
        return true;
    }
    return false;
}

bool conv(const std::string& from, float& to)
{
    try {
        to = std::stof(from.c_str());
    } catch (std::logic_error&) {
        return false;
    }
    return true;
}

bool conv(const std::string& from, double& to)
{
    try {
        to = std::stod(from.c_str());
    } catch (std::logic_error&) {
        return false;
    }
    return true;
}

//-------------------------------------------------------------------------------------------------

static const char* c_typeNames[] =
{
    "none",     // noType
    "bool",     // boolType
    "int",      // intType,
    "real",     // realType,
    "string",   // stringType,
    "enum",     // enumType,
    "array",    // arrayType,
    "user",     //userType
};

const char* typeAsString(Type t)
{
    return c_typeNames[t];
}

} // namespace detail
} // namespace ponder
