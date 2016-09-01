/****************************************************************************
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Copyright (C) 2015-2016 Billy Quith.
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


#ifndef PONDER_DETAIL_FUNCTIONIMPL_HPP
#define PONDER_DETAIL_FUNCTIONIMPL_HPP

#include <ponder/function.hpp>
//#include <ponder/detail/objecttraits.hpp>
#include <ponder/detail/functiontraits.hpp>
#include <ponder/valuemapper.hpp>

namespace ponder {
namespace detail {

    
struct FunctionMapParamsToValueType
{
    typedef std::vector<ValueType> ReturnType;
    
    template <typename T>
    static ValueType apply() { return mapType<T>(); }
};
    
    
// Apply B to canonical function F = R(A...)
template <typename F, typename B> struct FunctionApplyToParams;
    
template <typename B, typename... A>
struct FunctionApplyToParams<std::tuple<A...>, B>
{
    static typename B::ReturnType foreach()
    {
        return typename B::ReturnType { B::template apply<A>()... };
    }
};
    
template <typename B>
struct FunctionApplyToParams<std::tuple<void>, B>
{
    static typename B::ReturnType foreach()
    {
        return typename B::ReturnType { };
    }
};
    
template <typename T>
class FunctionImpl : public Function
{
    typedef T FuncTraits;
    
public:

    FunctionImpl(IdRef name) : Function(name)
    {
        m_name = name;
        m_funcType = FuncTraits::which;
        m_returnType = mapType<typename FuncTraits::ReturnType>();
        m_argTypes = FunctionApplyToParams<typename FuncTraits::Details::ParamTypes,
                                           FunctionMapParamsToValueType>::foreach();
    }
};

template <typename F>
static inline Function* newFunction(IdRef name, F function)
{
    typedef detail::FunctionTraits<F> FuncTraits;
    
    static_assert(FuncTraits::which != FunctionType::None, "Type is not a function");
    
    return new FunctionImpl<FuncTraits>(name);
}

} // namespace detail
} // namespace ponder

#endif // PONDER_DETAIL_FUNCTIONIMPL_HPP
