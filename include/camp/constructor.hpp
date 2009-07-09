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


#ifndef CAMP_CONSTRUCTOR_HPP
#define CAMP_CONSTRUCTOR_HPP


#include <camp/config.hpp>
#include <camp/args.hpp>
#include <vector>


namespace camp
{
class Value;

/**
 * \brief camp::Constructor represents a metaconstructor which is used to create objects instances from metaclasses
 *
 * This class is an interface which has to be derived to implement typed constructors.
 *
 * \sa Property, Function
 */
class Constructor
{
public:

    /**
     * \brief Destructor
     */
    virtual ~Constructor() {}

    /**
     * \brief Check if the constructor matches the given set of arguments
     *
     * \param args Set of arguments to check
     *
     * \return True if the constructor is compatible with the given arguments
     */
    virtual bool matches(const Args& args) const = 0;

    /**
     * \brief Use the constructor to create a new object
     *
     * \param args Set of arguments to pass to the constructor
     *
     * \return Pointer to the new object, or 0 on failure
     */
    virtual void* create(const Args& args) const = 0;
};

} // namespace camp


#endif // CAMP_CONSTRUCTOR_HPP
