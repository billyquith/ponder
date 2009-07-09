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


#ifndef CAMP_DETAIL_VALUEIMPL_HPP
#define CAMP_DETAIL_VALUEIMPL_HPP

#include <camp/type.hpp>
#include <camp/enumobject.hpp>
#include <camp/userobject.hpp>
#include <camp/valuemapper.hpp>
#include <camp/invalidvalue.hpp>
#include <boost/variant/static_visitor.hpp>
#include <ostream>
#include <string>


namespace camp
{
namespace detail
{
/**
 * \brief Value visitor which converts the stored value to a type T
 */
template <typename T>
struct ConvertVisitor : public boost::static_visitor<T>
{
    template <typename U>
    T operator()(const U& value) const
    {
        // Dispatch to the proper ValueConverter
        return camp_ext::ValueMapper<T>::from(value);
    }

    T operator()(T value) const
    {
        // Optimization when source type is the same as requested type
        return value;
    }

    T operator()(const UserObject& value) const
    {
        // The source value is a user object: there's only one conversion possible
        return value.get<T>();
    }

    T operator()(NoType) const
    {
        // Error: trying to convert an empty value
        CAMP_ERROR(InvalidValue(noType, mapType<T>()));
    }
};

/**
 * \brief Specialization of ConvertVisitor for UserObject
 *
 * The generic version would work thanks to the ValueMapper<UserObject> specialization,
 * but it fails to compile because when T == UserObject there are two similar overloads of operator()
 * and the compiler gets confused.
 */
template <>
struct ConvertVisitor<UserObject> : public boost::static_visitor<UserObject>
{
    UserObject operator()(const UserObject& value) const
    {
        // The source value is a user object: just return it
        return value;
    }

    template <typename U>
    UserObject operator()(const U&) const
    {
        // Error: trying to convert a non-user type to a UserObject
        CAMP_ERROR(InvalidValue(mapType<U>(), userType));
    }
};

/**
 * \brief Binary value visitor which compares two values using operator <
 */
struct LessThanVisitor : public boost::static_visitor<bool>
{
    template <typename T, typename U>
    bool operator()(const T&, const U&) const
    {
        // Different types : compare types identifiers
        return mapType<T>() < mapType<U>();
    }

    template <typename T>
    bool operator()(const T& v1, const T& v2) const
    {
        // Same types : compare values
        return v1 < v2;
    }

    bool operator()(NoType, NoType) const
    {
        // No type (empty values) : they're considered equal
        return false;
    }
};

/**
 * \brief Binary value visitor which compares two values using operator ==
 */
struct EqualVisitor : public boost::static_visitor<bool>
{
    template <typename T, typename U>
    bool operator()(const T&, const U&) const
    {
        // Different types : not equal
        return false;
    }

    template <typename T>
    bool operator()(const T& v1, const T& v2) const
    {
        // Same types : compare values
        return v1 == v2;
    }

    bool operator()(NoType, NoType) const
    {
        // No type (empty values) : they're considered equal
        return true;
    }
};

/**
 * \brief Value visitor which prints the stored value to a standard stream
 */
struct PrintVisitor : public boost::static_visitor<>
{
    PrintVisitor(std::ostream& stream)
        : m_stream(stream)
    {
    }

    template <typename U>
    void operator()(const U& value) const
    {
        // Generic version for all compatible types
        m_stream << value;
    }

    void operator()(const EnumObject& value) const
    {
        // Enum object: print the value
        m_stream << value.value();
    }

    void operator()(const UserObject& value) const
    {
        // User object: print the metaclass name
        // @todo use the metaclass.toString() function if available
        m_stream << value.pointer();
    }

    void operator()(NoType) const
    {
        // No type: do nothing
    }

    // To remove VC++ warnings
    PrintVisitor& operator=(const PrintVisitor&);

    std::ostream& m_stream;
};

} // namespace detail

} // namespace camp


#endif // CAMP_DETAIL_VALUEIMPL_HPP
