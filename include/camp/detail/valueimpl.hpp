/****************************************************************************
**
** Copyright (C) 2009-2010 TECHNOGERMA Systems France and/or its subsidiary(-ies).
** Contact: Technogerma Systems France Information (contact@technogerma.fr)
**
** This file is part of the CAMP library.
**
** CAMP is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
** 
** CAMP is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
** 
** You should have received a copy of the GNU Lesser General Public License
** along with CAMP.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/


#ifndef CAMP_DETAIL_VALUEIMPL_HPP
#define CAMP_DETAIL_VALUEIMPL_HPP

#include <camp/type.hpp>
#include <camp/valuemapper.hpp>
#include <camp/badtype.hpp>
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

    T operator()(const T& value) const
    {
        // Optimization when source type is the same as requested type
        return value;
    }

    T operator()(NoType) const
    {
        // Error: trying to convert an empty value
        CAMP_ERROR(BadType(noType, mapType<T>()));
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

} // namespace detail

} // namespace camp


#endif // CAMP_DETAIL_VALUEIMPL_HPP
