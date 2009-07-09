/****************************************************************************
**
** Copyright (C) 2009 TECHNOGERMA Systems France and/or its subsidiary(-ies).
** Contact: Technogerma Systems France Information (contact@technogerma.fr)
**
** This file is part of the CAMP library.
**
** Commercial Usage
** Licensees holding valid CAMP Commercial licenses may use this file in
** accordance with the CAMP Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and TECHNOGERMA Systems France.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL3.txt included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at sales@technogerma.fr.
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
