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


#ifndef CAMP_QT_QSTRING_HPP
#define CAMP_QT_QSTRING_HPP


#include <camp/valuemapper.hpp>
#include <QString>
#include <string>


namespace camp_ext
{
/**
 * \brief Value mapper for QString
 *
 * This class tells CAMP how to handle QString as a string type
 */
template <>
struct ValueMapper<QString>
{
    /**
     * \brief CAMP type associated to QString
     */
    static const int type = camp::stringType;

    /**
     * \brief Convert from QString to std::string
     *
     * \param source Source QString instance to convert
     *
     * \return \a source converted to a std::string
     */
    static std::string to(const QString& source)
    {
        return source.toStdString();
    }

    /**
     * \brief Convert from any type to QString
     *
     * We simply reuse ValueMapper<std::string>.
     *
     * \param source Source value to convert
     *
     * \return \a source converted to a QString
     */
    template <typename T>
    static QString from(const T& source)
    {
        return QString::fromStdString(ValueMapper<std::string>::from(source));
    }
};

} // namespace camp_ext


#endif // CAMP_QT_QSTRING_HPP
