/****************************************************************************
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Copyright (C) 2015-2020 Nick Trout.
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

#pragma once
#ifndef PONDER_VALUE_REF_HPP
#define PONDER_VALUE_REF_HPP

#include <ponder/config.hpp>
#include <ponder/type.hpp>

namespace ponder {
namespace detail {

class ValueRef
{
public:

    template <typename T>
    static ValueRef make(T* p)
    {
        return ValueRef(p, Type<T>::info());
    }

    template <typename T>
    static ValueRef make(T& p)
    {
        return ValueRef(&p, std::type_index(typeid(T)));
    }

    ValueRef(const ValueRef&) = default;

    ValueRef& operator= (const ValueRef&) = delete;

    bool operator< (const ValueRef& other) const
    {
        return m_type->less(m_ptr, other.m_ptr);
    }

    bool operator== (const ValueRef& other) const
    {
        return m_type->equal(m_ptr, other.m_ptr);
    }

    template <typename T>
    T* getRef()
    {
        return static_cast<T*>(const_cast<void*>(m_ptr));
    }

    template <typename T>
    const T* getRef() const
    {
        return static_cast<const T*>(m_ptr);
    }

private:

    struct IType
    {
        virtual ~IType() {}
        virtual bool less(const void *a, const void *b) const = 0;
        virtual bool equal(const void *a, const void *b) const = 0;
    };

    template <typename T>
    struct Type final : public IType
    {
        static const IType* info()
        {
            static const Type<T> i;
            return &i;
        }

        bool less(const void *a, const void *b) const final
        {
            return *static_cast<const T*>(a) < *static_cast<const T*>(b);
        }
        bool equal(const void *a, const void *b) const final
        {
            return *static_cast<const T*>(a) == *static_cast<const T*>(b);
        }
    };

    ValueRef(void *p, const IType *t)
    :   m_ptr(p)
    ,   m_type(t)
    {}

    const void * const m_ptr;
    //std::type_index m_type;
    const IType * const m_type;
};

} // detail
} // Ponder

#endif // PONDER_VALUE_REF_HPP
