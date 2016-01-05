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


#ifndef PONDER_DETAIL_ISSMARTPOINTER_HPP
#define PONDER_DETAIL_ISSMARTPOINTER_HPP


#include <ponder/detail/yesnotype.hpp>
#include <type_traits>
#include <memory>


namespace ponder
{

template<class T>
T* get_pointer(T *p)
{
    return p;
}
    
template<class T>
T* get_pointer(std::auto_ptr<T> const& p)
{
    return p.get();
}

template<class T>
T* get_pointer(std::unique_ptr<T> const& p)
{
    return p.get();
}

template<class T>
T* get_pointer(std::shared_ptr<T> const& p)
{
    return p.get();
}

namespace detail
{
/**
 * \brief Utility class which tells at compile-time if a type T is a smart pointer to a type U
 *
 * To detect a smart pointer type, we check using SFINAE if T implements an operator -> returning a U*
 */
template <typename T, typename U>
struct IsSmartPointer
{
    enum { value = (!std::is_pointer<T>::value && !std::is_same<T, U>::value) };
};

} // namespace detail

} // namespace ponder

#endif // PONDER_DETAIL_ISSMARTPOINTER_HPP
