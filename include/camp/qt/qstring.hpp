/****************************************************************************
**
** Copyright (C) 2009-2010 TECHNOGERMA Systems France and/or its subsidiary(-ies).
** Contact: Technogerma Systems France Information (contact@technogerma.fr)
**
** This file is part of the CAMP library.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2010 TECHNOGERMA Systems France and/or its subsidiary(-ies).
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
** 
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
** 
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
** THE SOFTWARE.
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
