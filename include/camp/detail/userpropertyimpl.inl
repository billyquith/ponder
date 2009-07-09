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
