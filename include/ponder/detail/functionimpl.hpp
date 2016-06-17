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
#include <vector>
#include <string>
#include <utility>

namespace ponder {
namespace detail {

using namespace std::placeholders;

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
inline typename std::remove_reference<T>::type convertArg(const Args& args,
                                                          std::size_t index,
                                                          const std::string& function)
{
    try
    {
        return args[index].to<typename std::remove_reference<T>::type>();
    }
    catch (const BadType&)
    {
        PONDER_ERROR(BadArgument(args[index].type(), mapType<T>(), index, function));
    }
}


template <typename R, typename C>
struct CallHelper
{
    template<typename F, typename... A, std::size_t... Is>
    static Value call(F func,
                      C obj,
                      const Args& args,
                      const std::string& name,
                      index_sequence<Is...>)
    {
        return func(obj, convertArg<A>(args, Is, name)...);
    }

    template <typename F, typename... A>
    static Value call(F func, C obj, const Args& args, const std::string& name)
    {
        return call<F, A...>(func, obj, args, name, make_index_sequence<sizeof...(A)>());
    }
};

/*
 * Specialization of CallHelper for functions returning void
 */
template <typename C>
struct CallHelper<void, C>
{
    template<typename F, typename... A, std::size_t... Is>
    static Value call(F func,
                      C obj,
                      const Args& args,
                      const std::string& name,
                      index_sequence<Is...>)
    {
        return func(obj, convertArg<A>(args,Is,name)...), Value::nothing;
    }
    
    template <typename F, typename... A>
    static Value call(F func, C obj, const Args& args, const std::string& name)
    {
        return call<F, A...>(func, obj, args, name, make_index_sequence<sizeof...(A)>());
    }
};

    
/**
 * \class ponder::detail::FunctionImpl
 *
 * \brief Typed specialization of ponder::Function.
 *
 * The FunctionImpl class is a template which is specialized
 * according to the underlying function prototype.
 */

template <typename F1, typename F2 = void> class FunctionImpl;

    
/*
 * Specialization of FunctionImpl for all functions.
 */
template <typename C, typename R, typename... A>
class FunctionImpl<R (C, A...)> : public Function
{
public:

    /**
     * \brief Constructor
     */
    FunctionImpl(const std::string& name, std::function<R (C, A...)> function)
        :   Function(name, mapType<R>(), std::vector<ValueType> { mapType<A>()... })
        ,   m_function(function)
    {
    }

protected:
    
    /**
     * \see Function::execute
     */
    Value execute(const UserObject& object, const Args& args) const override
    {
        return CallHelper<R, C>::template call<decltype(m_function), A...>(m_function,
                                                                           object.get<C>(),
                                                                           args,
                                                                           name());
    }

private:

    std::function<R (C, A...)> m_function; ///< Object containing the actual function to call
};

} // namespace detail
} // namespace ponder

#endif // PONDER_DETAIL_FUNCTIONIMPL_HPP
