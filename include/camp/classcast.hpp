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


#ifndef CAMP_CLASSCAST_HPP
#define CAMP_CLASSCAST_HPP


#include <camp/config.hpp>


namespace camp
{
class Class;

/**
 * \brief Convert a pointer from a source metaclass to a related target metaclass
 *
 * \param pointer Source pointer to convert
 * \param sourceClass Source metaclass to convert from
 * \param targetClass Target metaclass to convert to
 *
 * \return Converted pointer, or 0 on failure
 */
CAMP_API void* classCast(void* pointer, const Class& sourceClass, const Class& targetClass);

} // namespace camp

#endif // CAMP_CLASSCAST_HPP
