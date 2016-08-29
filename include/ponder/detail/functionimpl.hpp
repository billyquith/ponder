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
#include <ponder/detail/objecttraits.hpp>
#include <ponder/detail/functiontraits.hpp>

namespace ponder {
namespace detail {

// Is T a user type.
template <typename T> struct IsUserType {
    static constexpr bool value = std::is_class<T>::value
                && !std::is_same<typename RawType<T>::Type, Value>::value
                && !std::is_same<typename RawType<T>::Type, std::string>::value;
};

// Decide whether the UserObject holder should be ref (true) or copy (false).
template <typename T> struct IsUserObjRef {
    static constexpr bool value = std::is_pointer<T>::value || std::is_reference<T>::value;
};


/*
 *  The ValueMapper assumes all user objects converted to UserObjects are references,
 *  however, this is not correct if we return by value, i.e. Obj foo(); as opposed
 *  to: const Obj& foo();
 *  This template ensures that we return the correct UserObject holder type.
 */
template <typename T, typename U = void>
struct CallReturner
{
    static inline Value value(const T& o) {return Value(o);}
};

template <typename T>
struct CallReturner<T,
    typename std::enable_if< IsUserType<T>::value && !IsUserObjRef<T>::value >::type>
{
    static inline UserObject value(const T& o) {return UserObject::copy(o);}
};

template <typename T>
struct CallReturner<T,
    typename std::enable_if< IsUserType<T>::value && IsUserObjRef<T>::value >::type>
{
    static inline UserObject value(const T& o) {return UserObject::ref(o);}
};

    
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
        PONDER_ERROR(BadArgument(args[index].type(), mapType<T>(), index, function));
    }
}

/*
 * Object function call helper to allow specialisation by return type.
 */
template <typename R, typename C>
class CallObjectHelper
{
    template<typename F, typename... A, std::size_t... Is>
    static Value call(F func, C obj, const Args& args, IdRef name, index_sequence<Is...>)
    {
        return CallReturner<R>::value(func(obj, convertArg<A>(args, Is, name)...));
    }

public:
    
    template <typename F, typename... A>
    static Value call(F func, C obj, const Args& args, IdRef name)
    {
        return call<F, A...>(func, obj, args, name, make_index_sequence<sizeof...(A)>());
    }
};

/*
 * Specialization of CallObjectHelper for functions returning void
 */
template <typename C>
class CallObjectHelper<void, C>
{
    template<typename F, typename... A, std::size_t... Is>
    static Value call(F func, C obj, const Args& args, IdRef name, index_sequence<Is...>)
    {
        func(obj, convertArg<A>(args,Is,name)...);
        return Value::nothing;
    }

public:
    template <typename F, typename... A>
    static Value call(F func, C obj, const Args& args, IdRef name)
    {
        return call<F, A...>(func, obj, args, name, make_index_sequence<sizeof...(A)>());
    }
};
    

/*
 * Static function call helper to allow specialisation by return type.
 */
template <typename R>
class CallStaticHelper
{
    template<typename F, typename... A, std::size_t... Is>
    static Value call(F func, const Args& args, IdRef name, index_sequence<Is...>)
    {
        return CallReturner<R>::value(func(convertArg<A>(args, Is, name)...));
    }
    
public:
    
    template <typename F, typename... A>
    static Value call(F func, const Args& args, IdRef name)
    {
        return call<F, A...>(func, args, name, make_index_sequence<sizeof...(A)>());
    }
};

/*
 * Specialization of CallStaticHelper for functions returning void
 */
template <>
class CallStaticHelper<void>
{
    template<typename F, typename... A, std::size_t... Is>
    static Value call(F func, const Args& args, IdRef name, index_sequence<Is...>)
    {
        func(convertArg<A>(args,Is,name)...);
        return Value::nothing;
    }
    
public:
    template <typename F, typename... A>
    static Value call(F func, const Args& args, IdRef name)
    {
        return call<F, A...>(func, args, name, make_index_sequence<sizeof...(A)>());
    }
};
    

enum FunctionImplType
{
    FuncImplFunctionWrapper,
    FuncImplStaticFunction,
};
    
/**
 * \class ponder::detail::FunctionImpl
 *
 * \brief Typed specialization of ponder::Function.
 *
 * The FunctionImpl class is a template which is specialized
 * according to the underlying function prototype.
 */
template <int F, typename T = void> class FunctionImpl;

template <typename C, typename R, typename... A>
class FunctionImpl<FuncImplFunctionWrapper, R (C, A...)> : public Function
{
public:
    
    FunctionImpl(IdRef name, std::function<R (C, A...)> function)
    :   Function(name, mapType<R>(), std::vector<ValueType> { mapType<A>()... })
    ,   m_function(function)
    {}
    
protected:
    
    /// \see Function::execute
    Value execute(const UserObject& object, const Args& args) const override
    {
        return CallObjectHelper<R, C>::template
            call<decltype(m_function), A...>(m_function, object.get<C>(), args, name());
    }
    
private:
    
    std::function<R (C, A...)> m_function; ///< Object containing the actual function to call
};

    
template <typename R, typename... A>
class FunctionImpl<FuncImplStaticFunction, R (A...)> : public Function
{
public:
    
    typedef R(*FuncType)(A...);
    
    FunctionImpl(IdRef name, FuncType function)
    :   Function(name, mapType<R>(), std::vector<ValueType> { mapType<A>()... })
    ,   m_function(function)
    {}
    
protected:
    
    /// \see Function::execute
    Value execute(const UserObject&, const Args& args) const override
    {
        return CallStaticHelper<R>::template
            call<decltype(m_function), A...>(m_function, args, name());
    }
    
private:
    
    FuncType m_function; ///< Object containing the actual function to call
};
    

// Map from function traits type (FunctionType) to function implementation type.
// We do this to handle differing function types, e.g. static function have no instance
// parameter.
template <int FuncTypeWhich> struct FuncImplTypeMap;
    
template <> struct FuncImplTypeMap<(int)FunctionType::Function>
{ static constexpr int Type = FuncImplStaticFunction; };

template <> struct FuncImplTypeMap<(int)FunctionType::FunctionWrapper>
{ static constexpr int Type = FuncImplFunctionWrapper; };

template <> struct FuncImplTypeMap<(int)FunctionType::Lambda>
{ static constexpr int Type = FuncImplFunctionWrapper; };

template <> struct FuncImplTypeMap<(int)FunctionType::MemberFunction>
{ static constexpr int Type = FuncImplFunctionWrapper; };

    
} // namespace detail
} // namespace ponder

#endif // PONDER_DETAIL_FUNCTIONIMPL_HPP
