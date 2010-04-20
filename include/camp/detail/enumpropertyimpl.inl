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


namespace camp
{
namespace detail
{
//-------------------------------------------------------------------------------------------------
template <typename A>
EnumPropertyImpl<A>::EnumPropertyImpl(const std::string& name, const A& accessor)
    : EnumProperty(name, enumByType<typename A::DataType>())
    , m_accessor(accessor)
{
}

//-------------------------------------------------------------------------------------------------
template <typename A>
Value EnumPropertyImpl<A>::getValue(const UserObject& object) const
{
    return m_accessor.get(object.get<typename A::ClassType>());
}

//-------------------------------------------------------------------------------------------------
template <typename A>
void EnumPropertyImpl<A>::setValue(const UserObject& object, const Value& value) const
{
    if (!m_accessor.set(object.get<typename A::ClassType>(), value))
        CAMP_ERROR(InvalidAccess(name().c_str(), InvalidAccess::Write));
}

//-------------------------------------------------------------------------------------------------
template <typename A>
bool EnumPropertyImpl<A>::isReadable() const
{
    return A::canRead;
}

//-------------------------------------------------------------------------------------------------
template <typename A>
bool EnumPropertyImpl<A>::isWritable() const
{
    return A::canWrite;
}

} // namespace detail

} // namespace camp
