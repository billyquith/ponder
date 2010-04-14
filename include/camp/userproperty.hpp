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


#ifndef CAMP_USERPROPERTY_HPP
#define CAMP_USERPROPERTY_HPP


#include <camp/property.hpp>


namespace camp
{
class Class;

/**
 * \brief Specialized type of property for user types
 *
 */
class CAMP_API UserProperty : public Property
{
public:

    /**
     * \brief Construct the property from its description
     *
     * \param name Name of the property
     * \param propClass Eumeration the property is bound to
     */
    UserProperty(const std::string& name, const Class& propClass);

    /**
     * \brief Destructor
     */
    virtual ~UserProperty();

    /**
     * \brief Get the owner class
     *
     * \return Class the property is bound to
     */
    const Class& getClass() const;

    /**
     * \brief Accept the visitation of a ClassVisitor
     *
     * \param visitor Visitor to accept
     */
    virtual void accept(ClassVisitor& visitor) const;

private:

    const Class* m_class; ///< Owner class of the property
};

} // namespace camp


#endif // CAMP_ENUMPROPERTY_HPP
