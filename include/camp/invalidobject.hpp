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
** contact the sales department at contact@technogerma.fr.
**
****************************************************************************/


#ifndef CAMP_INVALIDOBJECT_HPP
#define CAMP_INVALIDOBJECT_HPP


#include <camp/error.hpp>


namespace camp
{
class UserObject;

/**
 * \brief Error thrown when using an invalid user object
 *
 * This error may be thrown if user object is null, of if its type doesn't match the requested one
 */
class CAMP_API InvalidObject : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param object User object
     */
    InvalidObject(const UserObject& object) throw();

    /**
     * \brief Destructor
     */
    virtual ~InvalidObject() throw();

    /**
     * \brief Return a message describing the error
     *
     * \return Pointer to a string describing the error
     */
    virtual const char* what() const throw();

    /**
     * \brief Return the user object
     *
     * \return Reference to the user object
     */
    const UserObject& object() const throw();

private:

    const UserObject* m_object; ///< User object
};

} // namespace camp


#endif // CAMP_INVALIDOBJECT_HPP
