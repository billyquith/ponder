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


#ifndef CAMP_SIMPLEPROPERTY_HPP
#define CAMP_SIMPLEPROPERTY_HPP


#include <camp/property.hpp>


namespace camp
{
/**
 * \brief Base class for all simple types of properties
 *
 * This class actually does nothing more than its base, it's just a way to separate
 * simple properties from other types.
 *
 * \sa ArrayProperty, EnumProperty, ObjectProperty
 */
class CAMP_API SimpleProperty : public Property
{
public:

    /**
     * \brief Construct the property from its description
     *
     * \param name Name of the property
     * \param type Type of the property
     */
    SimpleProperty(const std::string& name, Type type);

    /**
     * \brief Destructor
     */
    virtual ~SimpleProperty();

    /**
     * \brief Accept the visitation of a ClassVisitor
     *
     * \param visitor Visitor to accept
     */
    virtual void accept(ClassVisitor& visitor) const;
};

} // namespace camp


#endif // CAMP_SIMPLEPROPERTY_HPP
