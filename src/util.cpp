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


#include <camp/util.hpp>

// Convert to string:
//  #include "format.h"
//  http://zverovich.net/2013/09/07/integer-to-string-conversion-in-cplusplus.html

// Convert from string:
//  http://tinodidriksen.com/2010/02/16/cpp-convert-string-to-int-speed/

namespace camp
{
namespace detail
{

//-------------------------------------------------------------------------------------------------
// parse string

bool conv(const std::string& from, char& to)
{
    to = std::atoi(from.c_str());
    return true;
}

bool conv(const std::string& from, unsigned char& to)
{
    to = std::atoi(from.c_str());
    return true;
}

bool conv(const std::string& from, short& to)
{
    to = std::atoi(from.c_str());
    return true;
}

bool conv(const std::string& from, unsigned short& to)
{
    to = std::atoi(from.c_str());
    return true;
}

bool conv(const std::string& from, int& to)
{
    to = std::atoi(from.c_str());
    return true;
}

bool conv(const std::string& from, unsigned int& to)
{
    to = std::atoi(from.c_str());
    return true;
}

bool conv(const std::string& from, long long& to)
{
    to = std::atoll(from.c_str());
    return true;
}

bool conv(const std::string& from, unsigned long long& to)
{
    to = std::atoll(from.c_str());
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
    to = std::atof(from.c_str());
    return true;
}

bool conv(const std::string& from, double& to)
{
    to = std::atof(from.c_str());
    return true;
}

//-------------------------------------------------------------------------------------------------
    
} // namespace util
} // namespace camp
