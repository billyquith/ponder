/****************************************************************************
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Contact: Tegesoft Information (contact@tegesoft.com)
**
** This file is part of the CAMP library.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
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


#ifndef CAMP_CONFIG_HPP
#define CAMP_CONFIG_HPP


// We define the CAMP_API macro according to the
// current operating system and build mode
#if defined(_WIN32) || defined(__WIN32__)

    #ifndef CAMP_STATIC

        // Windows platforms need specific keywords for import / export
        #ifdef CAMP_EXPORTS

            // From DLL side, we must export
            #define CAMP_API __declspec(dllexport)

        #else

            // From client application side, we must import
            #define CAMP_API __declspec(dllimport)

        #endif

    #else

        // No specific directive needed for static build
        #define CAMP_API

    #endif

#else

    // Other platforms don't need to define anything
    #define CAMP_API

#endif


// We disable some annoying warnings of VC++
#if defined(_MSC_VER)

    #pragma warning(disable: 4275) // non dll-interface class 'X' used as base for dll-interface class 'Y'
    #pragma warning(disable: 4251) // class 'X' needs to have dll-interface to be used by clients of class 'Y'

#endif


#endif // CAMP_CONFIG_HPP
