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


#ifndef CAMP_ENUMPROPERTY_HPP
#define CAMP_ENUMPROPERTY_HPP


#include <camp/property.hpp>


namespace camp
{
class Enum;

/**
 * \brief Specialized type of property for enums
 *
 */
class CAMP_API EnumProperty : public Property
{
public:

    /**
     * \brief Construct the property from its description
     *
     * \param name Name of the property
     * \param propEnum Eumeration the property is bound to
     */
    EnumProperty(const std::string& name, const Enum& propEnum);

    /**
     * \brief Destructor
     */
    virtual ~EnumProperty();

    /**
     * \brief Get the owner enum
     *
     * \return Enum the property is bound to
     */
    const Enum& getEnum() const;

    /**
     * \brief Accept the visitation of a ClassVisitor
     *
     * \param visitor Visitor to accept
     */
    virtual void accept(ClassVisitor& visitor) const;

private:

    const Enum* m_enum; ///< Owner enum of the property
};

} // namespace camp


#endif // CAMP_ENUMPROPERTY_HPP
