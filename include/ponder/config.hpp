/****************************************************************************
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Copyright (C) 2015-2018 Nick Trout.
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

#pragma once
#ifndef PONDER_CONFIG_HPP
#define PONDER_CONFIG_HPP

// Earlier MSVC compilers have C++0x bugs which cause problems for Ponder.
#if defined(_MSC_VER)
    static_assert(_MSC_VER >= 1900, "MSVC 2015 is required.");
    static_assert(_MSC_FULL_VER >= 190023918, "MSVC 2015 Update 2 required.");
#endif

// We define the PONDER_API macro according to the
// current operating system and build mode
#if defined(_WIN32) || defined(__WIN32__)

#   ifndef PONDER_STATIC
        // Windows platforms need specific keywords for import / export
#       ifdef PONDER_EXPORTS
            // From DLL side, we must export
#           define PONDER_API __declspec(dllexport)
#       else
            // From client application side, we must import
#           define PONDER_API __declspec(dllimport)
#       endif
#   else
        // No specific directive needed for static build
#       define PONDER_API
#   endif
#else
    // Other platforms don't need to define anything
#   define PONDER_API
#endif

#ifndef PONDER_USING_LUA
#   define PONDER_USING_LUA 0
#endif
    
// We disable some annoying warnings of VC++
#if defined(_MSC_VER)
    #pragma warning(disable: 4275) // non dll-interface class 'X' used as base for dll-interface class 'Y'
    #pragma warning(disable: 4251) // class 'X' needs to have dll-interface to be used by clients of class 'Y'
#endif

#if defined(__GNUC__) && __GNUC__ <= 4 && __GNUC_MINOR__ < 9
    // Workaround a bug in libstdc++ where erase() should accept const iterator
    // See https://gcc.gnu.org/bugzilla/show_bug.cgi?id=54577
    #define PONDER__WORKAROUND_GCC_N2350 1
#else
    #define PONDER__WORKAROUND_GCC_N2350 0
#endif

// If user doesn't define traits use the default:
#ifndef PONDER_ID_TRAITS_USER
//# define PONDER_ID_TRAITS_STD_STRING      // Use std::string and const std::string&
#   define PONDER_ID_TRAITS_STRING_VIEW     // Use std::string and ponder::string_view
#endif // PONDER_ID_TRAITS_USER

#include "detail/idtraits.hpp"
#include <cassert>

#define PONDER__NON_COPYABLE(CLS) \
    CLS(CLS const&) = delete; \
    CLS& operator=(CLS const&) = delete

#define PONDER__UNUSED(VAR) ((void)&(VAR))

#endif // PONDER_CONFIG_HPP

