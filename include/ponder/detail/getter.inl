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
template <typename T>
GetterInterface<T>::~GetterInterface()
{
}

template <typename T, typename C>
GetterImpl<T, C>::GetterImpl(std::function<T (C&)> function)
    : m_function(function)
{
}

template <typename T, typename C>
T GetterImpl<T, C>::get(const UserObject& object) const
{
    return m_function(object.get<C>());
}

template <typename T>
Getter<T>::Getter(const T& defaultValue)
    : m_getter()
    , m_defaultValue(defaultValue)
{
}

template <typename T>
template <typename C>
Getter<T>::Getter(std::function<T (C&)> function)
    : m_getter(new GetterImpl<T, C>(function))
{
}

template <typename T>
const T& Getter<T>::get() const
{
    return m_defaultValue;
}

template <typename T>
T Getter<T>::get(const UserObject& object) const
{
    return m_getter ? m_getter->get(object) : m_defaultValue;
}

} // namespace detail

} // namespace ponder
