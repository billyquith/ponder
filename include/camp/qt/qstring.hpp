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
