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


#ifndef CAMP_QT_QLIST_HPP
#define CAMP_QT_QLIST_HPP


#include <camp/arraymapper.hpp>
#include <QList>


namespace camp_ext
{
/*
 * \brief Specialization of ArrayMapper for QList
 */
 template <typename T>
 struct ArrayMapper<QList<T> >
 {
    typedef T ElementType;

    static bool dynamic()
    {
        return true;
    }

    static std::size_t size(const QList<T>& arr)
    {
        return static_cast<std::size_t>(arr.size());
    }

    static const T& get(const QList<T>& arr, std::size_t index)
    {
        return arr[static_cast<int>(index)];
    }

    static void set(QList<T>& arr, std::size_t index, const T& value)
    {
        arr[static_cast<int>(index)] = value;
    }

    static void insert(QList<T>& arr, std::size_t before, const T& value)
    {
        arr.insert(static_cast<int>(before), value);
    }

    static void remove(QList<T>& arr, std::size_t index)
    {
        arr.removeAt(static_cast<int>(index));
    }
};

} // namespace camp_ext


#endif // CAMP_QT_QLIST_HPP
