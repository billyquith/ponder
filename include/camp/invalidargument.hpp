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


#ifndef CAMP_INVALIDARGUMENT_HPP
#define CAMP_INVALIDARGUMENT_HPP


#include <camp/invalidvalue.hpp>


namespace camp
{
/**
 * \brief Error thrown when passing an invalid argument to a function call
 */
class CAMP_API InvalidArgument : public InvalidValue
{
public:

    /**
     * \brief Constructor
     *
     * \param sourceType Type of the source value
     * \param requestedType Type the value is being converted to
     * \param function Name of the owner function
     * \param index Index of the argument
     */
    InvalidArgument(Type sourceType, Type requestedType, const char* function, std::size_t index) throw();

    /**
     * \brief Destructor
     */
    virtual ~InvalidArgument() throw();

    /**
     * \brief Return a message describing the error
     *
     * \return Pointer to a string describing the error
     */
    virtual const char* what() const throw();

    /**
     * \brief Return the name of the owner function
     *
     * \return Name of the function
     */
    const char* function() const throw();

    /**
     * \brief Return the index of the argument
     *
     * \return Index of the argument within the function call
     */
    std::size_t index() const throw();

private:

    char m_function[256]; ///< Name of the owner function
    std::size_t m_index; ///< Index of the argument in the function call
};

} // namespace camp


#endif // CAMP_INVALIDARGUMENT_HPP
