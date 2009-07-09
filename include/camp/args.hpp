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


#ifndef CAMP_ARGS_HPP
#define CAMP_ARGS_HPP


#include <camp/config.hpp>
#include <vector>


namespace camp
{
class Value;

/**
 * \brief Wrapper for packing an arbitrary number of arguments into a single object
 *
 * camp::Args is defined as a list of arguments of any type (wrapped in camp::Value instances),
 * which can conveniently be passed to all the CAMP entities which may need an arbitrary number of arguments
 * in a uniform way.
 *
 * Arguments lists can be constructed on the fly using one of the constructors (accept up to 5 arguments):
 *
 * \code
 * camp::Args args(1, true, "hello", 5.24, myObject);
 * \endcode
 *
 * ... or appended one by one using the + and += operators:
 *
 * \code
 * camp::Args args;
 * args += 1;
 * args += true;
 * args += "hello";
 * args += 5.24;
 * args = args + myObject;
 * \endcode
 *
 */
class CAMP_API Args
{
public:

    /**
     * \brief Default constructor (empty list of argument)
     */
    Args();

    /**
     * \brief Construct the list with 1 argument
     *
     * \param a0 Argument to put in the list
     */
    Args(const Value& a0);

    /**
     * \brief Construct the list with 2 arguments
     *
     * \param a0 First argument to put in the list
     * \param a1 Second argument to put in the list
     */
    Args(const Value& a0, const Value& a1);

    /**
     * \brief Construct the list with 3 arguments
     *
     * \param a0 First argument to put in the list
     * \param a1 Second argument to put in the list
     * \param a2 Third argument to put in the list
     */
    Args(const Value& a0, const Value& a1, const Value& a2);

    /**
     * \brief Construct the list with 4 arguments
     *
     * \param a0 First argument to put in the list
     * \param a1 Second argument to put in the list
     * \param a2 Third argument to put in the list
     * \param a3 Fourth argument to put in the list
     */
    Args(const Value& a0, const Value& a1, const Value& a2, const Value& a3);

    /**
     * \brief Construct the list with 5 arguments
     *
     * \param a0 First argument to put in the list
     * \param a1 Second argument to put in the list
     * \param a2 Third argument to put in the list
     * \param a3 Fourth argument to put in the list
     * \param a4 Fifth argument to put in the list
     */
    Args(const Value& a0, const Value& a1, const Value& a2, const Value& a3, const Value& a4);

    /**
     * \brief Return the number of arguments contained in the list
     *
     * \return Size of the arguments list
     */
    std::size_t count() const;

    /**
     * \brief Overload of operator [] to access an argument from its index
     *
     * \param index Index of the argument to get
     *
     * \return Value of the index-th argument
     *
     * \throw InvalidIndex index is out of range
     */
    const Value& operator[](std::size_t index) const;

    /**
     * \brief Overload of operator + to concatenate a list and a new argument
     *
     * \param arg Argument to concatenate to the list
     *
     * \return New list
     */
    Args operator+(const Value& arg) const;

    /**
     * \brief Overload of operator += to append a new argument to the list
     *
     * \param arg Argument to append to the list
     *
     * \return Reference to this
     */
    Args& operator+=(const Value& arg);

public:

    /**
     * \brief Special instance representing an empty set of arguments
     */
    static const Args empty;

private:

    std::vector<Value> m_values; ///< List of the values
};

} // namespace camp


#endif // CAMP_ARGS_HPP
