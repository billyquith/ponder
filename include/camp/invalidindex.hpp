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


#ifndef CAMP_INVALIDINDEX_HPP
#define CAMP_INVALIDINDEX_HPP


#include <camp/error.hpp>


namespace camp
{
/**
 * \brief Error thrown when using an index which is out of bounds
 *
 * This error may be thrown by every function which takes an index in its parameters
 */
class CAMP_API InvalidIndex : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param index Invalid index
     * \param size Allowed size
     */
    InvalidIndex(std::size_t index, std::size_t size) throw();

    /**
     * \brief Destructor
     */
    virtual ~InvalidIndex() throw();

    /**
     * \brief Return a message describing the error
     *
     * \return Pointer to a string describing the error
     */
    virtual const char* what() const throw();

    /**
     * \brief Return the invalid index which was passed
     *
     * \return Index
     */
    std::size_t index() const throw();

    /**
     * \brief Return the allowed size
     *
     * \return Valid range
     */
    std::size_t size() const throw();

private:

    std::size_t m_index; ///< Invalid index
    std::size_t m_size; ///< Valid size
};

} // namespace camp


#endif // CAMP_INVALIDINDEX_HPP
