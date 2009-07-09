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


#ifndef CAMP_QT_QVECTOR_HPP
#define CAMP_QT_QVECTOR_HPP


#include <camp/arraymapper.hpp>
#include <QVector>


namespace camp_ext
{
/*
 * \brief Specialization of ArrayMapper for QVector
 */
 template <typename T>
 struct ArrayMapper<QVector<T> >
 {
    typedef T ElementType;

    static bool dynamic()
    {
        return true;
    }

    static std::size_t size(const QVector<T>& arr)
    {
        return static_cast<std::size_t>(arr.size());
    }

    static const T& get(const QVector<T>& arr, std::size_t index)
    {
        return arr[static_cast<int>(index)];
    }

    static void set(QVector<T>& arr, std::size_t index, const T& value)
    {
        arr[static_cast<int>(index)] = value;
    }

    static void insert(QVector<T>& arr, std::size_t before, const T& value)
    {
        arr.insert(static_cast<int>(before), value);
    }

    static void remove(QVector<T>& arr, std::size_t index)
    {
        arr.remove(static_cast<int>(index));
    }
};

} // namespace camp_ext


#endif // CAMP_QT_QVECTOR_HPP
