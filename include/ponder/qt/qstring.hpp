/****************************************************************************
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Contact: Tegesoft Information (contact@tegesoft.com)
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
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


#ifndef PONDER_QT_QSTRING_HPP
#define PONDER_QT_QSTRING_HPP


#include <ponder/valuemapper.hpp>
#include <QString>
#include <string>


namespace ponder_ext
{
/**
 * \brief Value mapper for QString
 *
 * This class tells Ponder how to handle QString as a string type
 */
template <>
struct ValueMapper<QString>
{
    /**
     * \brief Ponder type associated to QString
     */
    static const int type = ponder::stringType;

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

} // namespace ponder_ext


#endif // PONDER_QT_QSTRING_HPP
