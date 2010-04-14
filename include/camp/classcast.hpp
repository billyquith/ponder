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
