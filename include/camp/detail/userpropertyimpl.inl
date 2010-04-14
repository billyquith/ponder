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
/**
 * \brief Helper structure to construct a UserObject according
 *        to whether a type is a reference type or not
 *
 * The generic version assumes a non-reference type and stores
 * the object by copy.
 */
template <bool IsRef>
struct ToUserObject
{
    template <typename T>
    static UserObject get(const T& value)
    {
        return UserObject::copy(value);
    }
};

/**
 * \brief Specialization of ToUserObject for reference types
 *
 * This version stores the object by reference (no copy).
 */
template <>
struct ToUserObject<true>
{
    template <typename T>
    static UserObject get(const T& value)
    {
        return UserObject::ref(value);
    }

    template <typename T>
    static UserObject get(T& value)
    {
        return UserObject::ref(value);
    }
};

//-------------------------------------------------------------------------------------------------
template <typename A>
UserPropertyImpl<A>::UserPropertyImpl(const std::string& name, const A& accessor)
    : UserProperty(name, classByType<typename A::DataType>())
    , m_accessor(accessor)
{
}

//-------------------------------------------------------------------------------------------------
template <typename A>
Value UserPropertyImpl<A>::getValue(const UserObject& object) const
{
    return ToUserObject<A::Traits::isRef>::get(m_accessor.get(object.get<typename A::ClassType>()));
}

//-------------------------------------------------------------------------------------------------
template <typename A>
void UserPropertyImpl<A>::setValue(const UserObject& object, const Value& value) const
{
    if (!m_accessor.set(object.get<typename A::ClassType>(), value))
        CAMP_ERROR(InvalidAccess(name().c_str(), InvalidAccess::Write));
}

//-------------------------------------------------------------------------------------------------
template <typename A>
bool UserPropertyImpl<A>::isReadable() const
{
    return A::canRead;
}

//-------------------------------------------------------------------------------------------------
template <typename A>
bool UserPropertyImpl<A>::isWritable() const
{
    return A::canWrite;
}

} // namespace detail

} // namespace camp
