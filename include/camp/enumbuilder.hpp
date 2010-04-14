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


#ifndef CAMP_ENUMBUILDER_HPP
#define CAMP_ENUMBUILDER_HPP


#include <camp/config.hpp>
#include <boost/noncopyable.hpp>
#include <string>


namespace camp
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
class CAMP_API EnumBuilder
{
public:

    /**
     * \brief Construct the builder with a target metaenum
     *
     * \param target Target metaenum to construct
     */
    EnumBuilder(Enum& target);

    /**
     * \brief Add a new pair to the metaenum
     *
     * \param name Name of the pair
     * \param value Value of the pair
     */
    EnumBuilder& value(const std::string& name, long value);

private:

    Enum* m_target; ///< Target metaenum to construct
};

} // namespace camp


#endif // CAMP_ENUMBUILDER_HPP
