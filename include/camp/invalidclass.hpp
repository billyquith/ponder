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


#ifndef CAMP_INVALIDCLASS_HPP
#define CAMP_INVALIDCLASS_HPP


#include <camp/error.hpp>


namespace camp
{
/**
 * \brief Error thrown when requesting a metaclass which doesn't exist
 */
class CAMP_API InvalidClass : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param className Name of the requested metaclass
     */
    InvalidClass(const char* className) throw();

    /**
     * \brief Destructor
     */
    virtual ~InvalidClass() throw();

    /**
     * \brief Return a message describing the error
     *
     * \return Pointer to a string describing the error
     */
    virtual const char* what() const throw();

    /**
     * \brief Return the requested metaclass name
     *
     * \return Name of the metaclass
     */
    const char* className() const throw();

private:

    char m_className[256]; ///< Name of the requested metaclass
};

} // namespace camp


#endif // CAMP_INVALIDCLASS_HPP
