/****************************************************************************
**
** Copyright (C) 2009-2010 TECHNOGERMA Systems France and/or its subsidiary(-ies).
** Contact: Technogerma Systems France Information (contact@technogerma.fr)
**
** This file is part of the CAMP library.
**
** CAMP is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
** 
** CAMP is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
** 
** You should have received a copy of the GNU Lesser General Public License
** along with CAMP.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/


#ifndef CAMP_CONFIG_HPP
#define CAMP_CONFIG_HPP


// We define the CAMP_API macro according to the
// current operating system and build mode
#if defined(_WIN32) || defined(__WIN32__)

    // Windows platforms need specific keywords for import / export
    #ifdef CAMP_EXPORTS

        // From DLL side, we must export
        #define CAMP_API __declspec(dllexport)

    #else

        // From client application side, we must import
        #define CAMP_API __declspec(dllimport)

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
