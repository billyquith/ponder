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
