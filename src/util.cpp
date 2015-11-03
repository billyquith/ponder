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
#include "format.h"


namespace camp
{
namespace util
{

//-------------------------------------------------------------------------------------------------
// Convert to string:
//  http://zverovich.net/2013/09/07/integer-to-string-conversion-in-cplusplus.html
    
//bool, long, double, std::string, EnumObject, UserObject

std::string conv_str(bool from)
{
    return fmt::format("{}", from);
}

std::string conv_str(int from)
{
    return fmt::format("{}", from);
}

std::string conv_str(long from)
{
    return fmt::format("{}", from);
}

std::string conv_str(unsigned long from)
{
    return fmt::format("{}", from);
}

std::string conv_str(double from)
{
    return fmt::format("{}", from);
}

//-------------------------------------------------------------------------------------------------

int conv_int(bool from)
{
    return from ? 1 : 0;
}

//-------------------------------------------------------------------------------------------------
// Convert from string:
//  http://tinodidriksen.com/2010/02/16/cpp-convert-string-to-int-speed/

bool conv_bool(std::string const& from)
{
    const char *s = from.c_str();
    if (strcasecmp(s, "1")==0 || strcasecmp(s, "true")==0)
        return true;
    if (strcasecmp(s, "0")==0 || strcasecmp(s, "false")==0)
        return false;
    throw bad_lexical_cast();
}

    int conv_int(std::string const& from) { return 0; }
    long conv_long(std::string const& from) { return 0; }
    unsigned long conv_ulong(std::string const& from) { return 0; }
    double conv_double(std::string const& from) { return 0.0; }

// bool
    

    
} // namespace util
} // namespace camp
