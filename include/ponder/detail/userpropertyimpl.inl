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



namespace ponder
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

template <typename A>
UserPropertyImpl<A>::UserPropertyImpl(const std::string& name, const A& accessor)
    : UserProperty(name, classByType<typename A::DataType>())
    , m_accessor(accessor)
{
}

template <typename A>
Value UserPropertyImpl<A>::getValue(const UserObject& object) const
{
    return ToUserObject<A::Traits::isRef>::get(m_accessor.get(object.get<typename A::ClassType>()));
}

template <typename A>
void UserPropertyImpl<A>::setValue(const UserObject& object, const Value& value) const
{
    if (!m_accessor.set(object.get<typename A::ClassType>(), value))
        PONDER_ERROR(ForbiddenWrite(name()));
}

template <typename A>
bool UserPropertyImpl<A>::isReadable() const
{
    return A::canRead;
}

template <typename A>
bool UserPropertyImpl<A>::isWritable() const
{
    return A::canWrite;
}

} // namespace detail

} // namespace ponder
