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
