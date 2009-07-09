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


#ifndef CAMP_INVALIDVALUE_HPP
#define CAMP_INVALIDVALUE_HPP


#include <camp/error.hpp>
#include <camp/type.hpp>


namespace camp
{
/**
 * \brief Error thrown when using an invalid value
 *
 * This error may be thrown when a value can't be converted to the requested type
 */
class CAMP_API InvalidValue : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param sourceType Type of the source value
     * \param requestedType Type the value is being converted to
     */
    InvalidValue(Type sourceType, Type requestedType) throw();

    /**
     * \brief Destructor
     */
    virtual ~InvalidValue() throw();

    /**
     * \brief Return a message describing the error
     *
     * \return Pointer to a string describing the error
     */
    virtual const char* what() const throw();

    /**
     * \brief Return the type of the source value
     *
     * \return Source type
     */
    Type sourceType() const throw();

    /**
     * \brief Return the type the value is being converted to
     *
     * \return Requested type
     */
    Type requestedType() const throw();

private:

    Type m_sourceType; ///< Type of the source value
    Type m_requestedType; ///< Type the value is being converted to
};

} // namespace camp


#endif // CAMP_INVALIDVALUE_HPP
