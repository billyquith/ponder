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
    static inline Value&& value(R&& o) {return std::move(Value(o));}
};

template <typename R>
struct CallReturnCopy<R, typename std::enable_if<detail::IsUserType<R>::value>::type>
{
    static inline Value&& value(R&& o) {return std::move(Value(UserObject::makeCopy(std::forward<R>(o))));}
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
    static inline Value&& value(R&& o) {return std::move(Value(o));}
};

template <typename R>
struct CallReturnInternalRef<R,
    typename std::enable_if<
        detail::IsUserType<R>::value
        || std::is_same<typename detail::RawType<R>::Type, UserObject>::value
    >::type>
{
    static inline Value&& value(R&& o) {return std::move(Value(UserObject::makeRef(std::forward<R>(o))));}
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
template <typename T>
inline typename std::remove_reference<T>::type
convertArg(const Args& args, std::size_t index, IdRef function)
{
    try {
        return args[index].to<typename std::remove_reference<T>::type>();
    }
    catch (const BadType&) {
        PONDER_ERROR(BadArgument(args[index].kind(), mapType<T>(), index, function));
    }
}

//-----------------------------------------------------------------------------
// Object function call helper to allow specialisation by return type.

template <typename R, typename C, typename Policies_t>
class CallObjectHelper
{
    template<typename F, typename... A, std::size_t... Is>
    static Value call(F func, C obj, const Args& args, IdRef name, detail::index_sequence<Is...>)
    {
        typedef typename ChooseCallReturner<Policies_t, R>::type CallReturner;
        return CallReturner::value(func(obj, convertArg<A>(args, Is, name)...));
    }

public:
    
    template <typename F, typename... A>
    static Value call(F func, C obj, const Args& args, IdRef name)
    {
        return call<F, A...>(func, obj, args, name, detail::make_index_sequence<sizeof...(A)>());
    }
};

// Specialization of CallObjectHelper for functions returning void
template <typename Policies_t, typename C>
class CallObjectHelper<void, C, Policies_t>
{
    template<typename F, typename... A, std::size_t... Is>
    static Value call(F func, C obj, const Args& args, IdRef name, detail::index_sequence<Is...>)
    {
        func(obj, convertArg<A>(args,Is,name)...);
        return Value::nothing;
    }

public:
    template <typename F, typename... A>
    static Value call(F func, C obj, const Args& args, IdRef name)
    {
        return call<F, A...>(func, obj, args, name, detail::make_index_sequence<sizeof...(A)>());
    }
};
    

//-----------------------------------------------------------------------------
// Static function call helper to allow specialisation by return type.

template <typename R, typename Policies_t>
class CallStaticHelper
{
    template<typename F, typename... A, std::size_t... Is>
    static Value call(F func, const Args& args, IdRef name, detail::index_sequence<Is...>)
    {
        typedef typename ChooseCallReturner<Policies_t, R>::type CallReturner;
        return CallReturner::value(func(convertArg<A>(args, Is, name)...));
    }
    
public:
    
    template <typename F, typename... A>
    static Value call(F func, const Args& args, IdRef name)
    {
        return call<F, A...>(func, args, name, detail::make_index_sequence<sizeof...(A)>());
    }
};

// Specialization of CallStaticHelper for functions returning void
template <typename Policies_t>
class CallStaticHelper<void, Policies_t>
{
    template<typename F, typename... A, std::size_t... Is>
    static Value call(F func, const Args& args, IdRef name, detail::index_sequence<Is...>)
    {
        func(convertArg<A>(args,Is,name)...);
        return Value::nothing;
    }
    
public:
    template <typename F, typename... A>
    static Value call(F func, const Args& args, IdRef name)
    {
        return call<F, A...>(func, args, name, detail::make_index_sequence<sizeof...(A)>());
    }
};

//-----------------------------------------------------------------------------
// Base for runtime function caller

class FunctionCaller
{
public:
    FunctionCaller(const IdRef name) : m_name(name) {}
    FunctionCaller(const FunctionCaller&) = delete;
    virtual ~FunctionCaller() {}
    
    const IdRef name() const { return m_name; }
    
    virtual Value execute(const UserObject&, const Args& args) const = 0;
    
private:
    const IdRef m_name;
};
    
   
enum FunctionImplType
{
    FuncImplClassFunctionWrapper,
    FuncImplStaticFunction,
};
    
/*
 * The FunctionImpl class is a template which is specialized
 * according to the underlying function prototype.
 */
template <int E, typename Policies_t, typename T = void> class FunctionCallerImpl;
    
template <typename Policies_t, typename C, typename R, typename... A>
class FunctionCallerImpl<FuncImplClassFunctionWrapper, Policies_t, R (C, A...)> : public FunctionCaller
{
public:
    
    FunctionCallerImpl(IdRef name, std::function<R (C, A...)> function)
    :   FunctionCaller(name)
    ,   m_function(function)
    {}
    
protected:
    
    /// \see Function::execute
    Value execute(const UserObject& object, const Args& args) const
    {
        return CallObjectHelper<R, C, Policies_t>::template
            call<decltype(m_function), A...>(m_function, object.get<C>(), args, name());
    }
    
private:
    
    std::function<R (C, A...)> m_function; ///< Object containing the actual function to call
};

    
template <typename Policies_t, typename R, typename... A>
class FunctionCallerImpl<FuncImplStaticFunction, Policies_t, R (A...)> : public FunctionCaller
{
public:
    
    FunctionCallerImpl(IdRef name, std::function<R (A...)> function)
    :   FunctionCaller(name)
    ,   m_function(function)
    {}
    
protected:
    
    /// \see Function::execute
    Value execute(const UserObject& object, const Args& args) const
    {
        return CallStaticHelper<R, Policies_t>::template
            call<decltype(m_function), A...>(m_function, args, name());
    }
    
private:
    
    std::function<R (A...)> m_function; ///< Object containing the actual function to call
};

    
// Map from function traits type (FunctionKind) to function implementation type.
// We do this to handle differing function types, e.g. static function have no instance
// parameter.
template <int FuncTypeWhich, typename U=void> struct FuncImplTypeMap;
    
template <> struct FuncImplTypeMap<(int)FunctionKind::Function>
{ static constexpr int Type = FuncImplStaticFunction; };

template <> struct FuncImplTypeMap<(int)FunctionKind::FunctionWrapper>
{ static constexpr int Type = FuncImplStaticFunction; };

template <> struct FuncImplTypeMap<(int)FunctionKind::Lambda>
{ static constexpr int Type = FuncImplStaticFunction; };

template <> struct FuncImplTypeMap<(int)FunctionKind::MemberFunction>
{ static constexpr int Type = FuncImplClassFunctionWrapper; };


} // namespace impl
} // namespace runtime
} // namespace ponder

#endif // PONDER_USES_DETAIL_RUNTIME_HPP
