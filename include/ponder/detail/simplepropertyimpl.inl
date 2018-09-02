/****************************************************************************
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Copyright (C) 2015-2018 Nick Trout.
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


namespace ponder {
namespace detail {
    
template <typename A>
SimplePropertyImpl<A>::SimplePropertyImpl(IdRef name, A accessor)
    : SimpleProperty(name, mapType<typename A::DataType>())
    , m_accessor(accessor)
{
}

template <typename A>
Value SimplePropertyImpl<A>::getValue(const UserObject& object) const
{
    return Value{m_accessor.m_interface.getter(object.get<typename A::ClassType>())};
}

template <typename A>
void SimplePropertyImpl<A>::setValue(const UserObject& object, const Value& value) const
{
//    static_assert(!std::is_const<typename A::ClassType>::value, "Const class");
//    static_assert(!std::is_reference<typename A::AccessType>::value, "Refs not allowed");
    
    if (!m_accessor.m_interface.setter(object.ref<typename A::ClassType>(), value.to<typename A::DataType>()))
        PONDER_ERROR(ForbiddenWrite(name()));
}

template <typename A>
bool SimplePropertyImpl<A>::isReadable() const
{
    return A::canRead;
}

template <typename A>
bool SimplePropertyImpl<A>::isWritable() const
{
    return A::canWrite;
}

} // namespace detail
} // namespace ponder
