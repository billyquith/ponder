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


#ifndef CAMP_INVALIDCONSTRUCTION_HPP
#define CAMP_INVALIDCONSTRUCTION_HPP


#include <camp/args.hpp>
#include <camp/error.hpp>


namespace camp
{
class Class;

/**
 * \brief Error thrown when an invalid call to Class::construct is made
 *
 * This exception may be thrown by Class::construct if no constructor matches
 * the list of arguments, or if the target type is unrelated to the metaclass.
 */
class CAMP_API InvalidConstruction : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param args List of arguments passed to the constructor
     * \param ownerClass Reference to the owner metaclass
     */
    InvalidConstruction(const Args& args, const Class& ownerClass) throw();

    /**
     * \brief Destructor
     */
    virtual ~InvalidConstruction() throw();

    /**
     * \brief Return a message describing the error
     *
     * \return Pointer to a string describing the error
     */
    virtual const char* what() const throw();

    /**
     * \brief Get the list of arguments
     *
     * \return Arguments that were passed to the constructor
     */
    const Args& args() const throw();

    /**
     * \brief Return the owner metaclass
     *
     * \return Reference to the owner metaclass
     */
    const Class& ownerClass() const throw();

private:

    Args m_args; ///< Arguments that were passed to the constructor
    const Class* m_ownerClass; ///< Owner metaclass
};

} // namespace camp


#endif // CAMP_INVALIDCONSTRUCTION_HPP
