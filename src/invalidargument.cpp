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


#include <camp/invalidargument.hpp>
#include <string.h>


namespace camp
{
//-------------------------------------------------------------------------------------------------
InvalidArgument::InvalidArgument(Type sourceType, Type requestedType, const char* function, std::size_t index) throw()
    : InvalidValue(sourceType, requestedType)
    , m_index(index)
{
    strncpy(m_function, function ? function : "", sizeof(m_function));
}

//-------------------------------------------------------------------------------------------------
InvalidArgument::~InvalidArgument() throw()
{
}

//-------------------------------------------------------------------------------------------------
const char* InvalidArgument::what() const throw()
{
    return "Invalid argument passed to function call; failed to convert to requested type";
}

//-------------------------------------------------------------------------------------------------
const char* InvalidArgument::function() const throw()
{
    return m_function;
}

//-------------------------------------------------------------------------------------------------
std::size_t InvalidArgument::index() const throw()
{
    return m_index;
}

} // namespace camp
