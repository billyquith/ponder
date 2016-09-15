/****************************************************************************
**
** This file is part of the Ponder library.
**
** The MIT License (MIT)
**
** Copyright (C) 2016 Bill Quith.
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

#ifndef PONDER_USES_DETAIL_RUNTIME_HPP
#define PONDER_USES_DETAIL_RUNTIME_HPP

#include <ponder/detail/rawtype.hpp>

namespace ponder {
namespace runtime {
namespace impl {

//-----------------------------------------------------------------------------
// Handle returning copies
    
template <typename R, typename U = void> struct CallReturnCopy;

template <typename R>
struct CallReturnCopy<R, typename std::enable_if<!detail::IsUserType<R>::value>::type>
{
    static inline Value value(const R& o) {return Value(o);}
};

template <typename R>
struct CallReturnCopy<R, typename std::enable_if<detail::IsUserType<R>::value>::type>
{
    static inline UserObject value(const R& o) {return UserObject::makeCopy(o);}
};

//-----------------------------------------------------------------------------
// Handle returning internal references
    
template <typename R, typename U = void> struct CallReturnInternalRef;

template <typename R>
struct CallReturnInternalRef<R,
    typename std::enable_if<
        !detail::IsUserType<R>::value
        && !std::is_same<typename detail::RawType<R>::Type, UserObject>::value
    >::type>
{
    static inline Value value(const R& o) {return Value(o);}
};

template <typename R>
struct CallReturnInternalRef<R,
    typename std::enable_if<
        detail::IsUserType<R>::value
        || std::is_same<typename detail::RawType<R>::Type, UserObject>::value
    >::type>
{
    static inline UserObject value(const R& o) {return UserObject::makeRef(o);}
};

//-----------------------------------------------------------------------------
// Choose which returner to use, based on policy
//  - map policy kind to actionable policy type
    
template <typename Policies_t, typename R> struct ChooseCallReturner;

template <typename... Ps, typename R>
struct ChooseCallReturner<std::tuple<policy::ReturnCopy, Ps...>, R>
{
    typedef CallReturnCopy<R> type;
};

template <typename... Ps, typename R>
struct ChooseCallReturner<std::tuple<policy::ReturnInternalRef, Ps...>, R>
{
    typedef CallReturnInternalRef<R> type;
};

template <typename R>
    struct ChooseCallReturner<std::tuple<>, R> // default
{
    typedef CallReturnCopy<R> type;
};

template <typename P, typename... Ps, typename R>
struct ChooseCallReturner<std::tuple<P, Ps...>, R> // recurse
{
    typedef typename ChooseCallReturner<std::tuple<Ps...>, R>::type type;
};

//-----------------------------------------------------------------------------
    
/**
 * \brief Helper function which converts an argument to a C++ type
 *
 * The main purpose of this function is to convert any BadType error to
 * a BadArgument one.
 *
 * \param args List of arguments
 * \param index Index of the argument to convert
 * \param function Name of the calling function
 *
 * \return Value of args[index] converted to T
 *
 * \thrown BadArgument conversion triggered a BadType error
 */
 
template <int TFrom, typename TTo>
struct ConvertArg
{
    static inline typename std::remove_reference<TTo>::type
    convert(const Args& args, std::size_t index)
    {
        try {
            return args[index].to<typename std::remove_reference<TTo>::type>();
        }
        catch (const BadType&) {
            PONDER_ERROR(BadArgument(args[index].kind(), mapType<TTo>(), index, "?"));
        }
    }
};

template <typename TTo>
struct ConvertArg<(int)ValueKind::User, TTo&>
{
    static inline TTo&
    convert(const Args& args, std::size_t index)
    {
        auto&& uobj = const_cast<Value&>(args[index]).ref<UserObject>();
        if (uobj.pointer() == nullptr)
            PONDER_ERROR(NullObject(&uobj.getClass()));
        return uobj.ref<TTo>();
    }
};

template <typename TTo>
struct ConvertArg<(int)ValueKind::User, const TTo&>
{
    static inline const TTo&
    convert(const Args& args, std::size_t index)
    {
        return args[index].cref<UserObject>().cref<TTo>();
    }
};

//-----------------------------------------------------------------------------
// Object function call helper to allow specialisation by return type. Applies policies.

template <typename R, typename FTraits, typename FPolicies>
class CallHelper
{
public:

    template<typename F, typename... A, std::size_t... Is>
    static Value call(F func, const Args& args, detail::index_sequence<Is...>)
    {
        typedef typename ChooseCallReturner<FPolicies, R>::type CallReturner;
        
        return CallReturner::value(
            func(ConvertArg<(int)ponder_ext::ValueMapper<typename detail::RawType<A>::Type>::type,
                            A>::convert(args, Is)...));
    }
};

// Specialization of CallHelper for functions returning void
template <typename FTraits, typename FPolicies>
class CallHelper<void, FTraits, FPolicies>
{
public:

    template<typename F, typename... A, std::size_t... Is>
    static Value call(F func, const Args& args, detail::index_sequence<Is...>)
    {
        func(ConvertArg<(int)ponder_ext::ValueMapper<typename detail::RawType<A>::Type>::type,
                        A>::convert(args,Is)...);
        return Value::nothing;
    }
};

//-----------------------------------------------------------------------------
// Convert traits to callable function wrapper. Generic for all function types.

template <typename R, typename A> struct FunctionWrapper;

template <typename R, typename... A> struct FunctionWrapper<R, std::tuple<A...>>
{
    typedef typename std::function<R(A...)> Type;
    
    template <typename F, typename FTraits, typename FPolicies>
    static Value call(F func, const Args& args)
    {
        return CallHelper<R, FTraits, FPolicies>::template
            call<F, A...>(func, args, detail::make_index_sequence<sizeof...(A)>());
    }
};
    
//-----------------------------------------------------------------------------
// Base for runtime function caller

class FunctionCaller
{
public:
    FunctionCaller(const IdRef name) : m_name(name) {}
    
    FunctionCaller(const FunctionCaller&) = delete; // no copying
    virtual ~FunctionCaller() {}
    
    const IdRef name() const { return m_name; }
    
    virtual Value execute(const Args& args) const = 0;
    
private:
    const IdRef m_name;
};

/*
 * The FunctionImpl class is a template which is specialized
 * according to the underlying function prototype.
 */
//template <int E, typename Policies_t, typename T = void> class FunctionCallerImpl;
    
template <typename F, typename FTraits, typename FPolicies>
class FunctionCallerImpl : public FunctionCaller
{
public:

    FunctionCallerImpl(IdRef name, F function)
    :   FunctionCaller(name)
    ,   m_function(function)
    {}
    
private:

    typedef typename FTraits::Details::FunctionCallTypes CallTypes;
    typedef FunctionWrapper<typename FTraits::ReturnType, CallTypes> FunctionType;
    
    typename FunctionType::Type m_function; // Object containing the actual function to call
    
    Value execute(const Args& args) const
    {
        return FunctionType::template call<decltype(m_function), FTraits, FPolicies>(m_function, args);
    }
};

} // namespace impl
} // namespace runtime
} // namespace ponder

#endif // PONDER_USES_DETAIL_RUNTIME_HPP
