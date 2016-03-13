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


#ifndef PONDER_ENUMBUILDER_HPP
#define PONDER_ENUMBUILDER_HPP

#include <ponder/config.hpp>
#include <string>


namespace ponder
{
    
class Enum;

/**
 * \brief Proxy class which fills a metaenum with its members
 *
 * This class is returned by Enum::declare<T> in order construct a
 * new metaenum. It contains functions to declare <name, value> pairs to
 * fill the metaenum.
 *
 * This class should never be explicitely instanciated, unless you
 * need to split the metaenum creation in multiple parts.
 */
class PONDER_API EnumBuilder
{
public:

    /**
     * \brief Construct the builder with a target metaenum
     *
     * \param target Target metaenum to construct
     */
    explicit EnumBuilder(Enum& target);

    /**
     * \brief Add a new pair to the metaenum
     *
     * \param name Name of the pair
     * \param value Value of the pair
     */
    EnumBuilder& value(const std::string& name, long value);
    
    /**
     * \brief Add a new pair to the metaenum using enum class
     *
     * e.g. `value("one", MyEnum::One)`
     *
     * \param name Name of the pair
     * \param enumValue Value of the pair
     */
    template <typename E>
    EnumBuilder& value(const std::string& name, E enumValue)
    {
        return value(name, static_cast<long>(enumValue));
    }

private:

    Enum* m_target; ///< Target metaenum to construct
};

} // namespace ponder


#endif // PONDER_ENUMBUILDER_HPP
