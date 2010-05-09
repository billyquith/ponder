/****************************************************************************
**
** Copyright (C) 2009-2010 TECHNOGERMA Systems France and/or its subsidiary(-ies).
** Contact: Technogerma Systems France Information (contact@technogerma.fr)
**
** This file is part of the CAMP library.
**
** CAMP is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
** 
** CAMP is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
** 
** You should have received a copy of the GNU Lesser General Public License
** along with CAMP.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/


#ifndef CAMP_DETAIL_FUNCTIONIMPL_HPP
#define CAMP_DETAIL_FUNCTIONIMPL_HPP


#include <camp/function.hpp>
#include <camp/value.hpp>
#include <camp/badtype.hpp>
#include <camp/badargument.hpp>
#include <camp/detail/callhelper.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/assign/list_of.hpp>
#include <string>


namespace camp
{
namespace detail
{
using boost::assign::list_of;

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
inline typename boost::remove_reference<T>::type convertArg(const Args& args, std::size_t index, const std::string& function)
{
    try
    {
        return args[index].to<typename boost::remove_reference<T>::type>();
    }
    catch (BadType&)
    {
        CAMP_ERROR(BadArgument(args[index].type(), mapType<T>(), index, function));
    }
}


/**
 * \class camp::detail::FunctionImpl
 *
 * \brief Typed specialization of camp::Function.
 *
 * The FunctionImpl class is a template which is specialized
 * according to the underlying function prototype.
 */

template <typename F1, typename F2 = void> class FunctionImpl;

/*
 * Specialization of FunctionImpl for functions taking no argument
 */
template <typename C, typename R>
class FunctionImpl<R (C)> : public Function
{
public:

    /**
     * \brief Constructor
     */
    FunctionImpl(const std::string& name, boost::function<R (C)> function)
        : Function(name, mapType<R>())
        , m_function(function)
    {
    }

protected:

    /**
     * \see Function::execute
     */
    virtual Value execute(const UserObject& object, const Args&) const
    {
        return CallHelper<R, C>::call(m_function, object.get<C>());
    }

private:

    boost::function<R (C)> m_function; ///< Object containing the actual function to call
};

/*
 * Specialization of FunctionImpl for functions taking 1 argument
 */
template <typename C, typename R, typename A0>
class FunctionImpl<R (C, A0)> : public Function
{
public:

    /**
     * \brief Constructor
     */
    FunctionImpl(const std::string& name, boost::function<R (C, A0)> function)
        : Function(name, mapType<R>(), list_of(mapType<A0>()))
        , m_function(function)
    {
    }

protected:

    /**
     * \see Function::execute
     */
    virtual Value execute(const UserObject& object, const Args& args) const
    {
        return CallHelper<R, C>::call(m_function, object.get<C>()
                                      , convertArg<A0>(args, 0, name()));
    }

private:

    boost::function<R (C, A0)> m_function; ///< Object containing the actual function to call
};

/*
 * Specialization of FunctionImpl for functions taking 2 arguments
 */
template <typename C, typename R, typename A0, typename A1>
class FunctionImpl<R (C, A0, A1)> : public Function
{
public:

    /**
     * \brief Constructor
     */
    FunctionImpl(const std::string& name, boost::function<R (C, A0, A1)> function)
        : Function(name, mapType<R>(), list_of(mapType<A0>())(mapType<A1>()))
        , m_function(function)
    {
    }

protected:

    /**
     * \see Function::execute
     */
    virtual Value execute(const UserObject& object, const Args& args) const
    {
        return CallHelper<R, C>::call(m_function, object.get<C>()
                                      , convertArg<A0>(args, 0, name())
                                      , convertArg<A1>(args, 1, name()));
    }

private:

    boost::function<R (C, A0, A1)> m_function; ///< Object containing the actual function to call
};

/*
 * Specialization of FunctionImpl for functions taking 3 arguments
 */
template <typename C, typename R, typename A0, typename A1, typename A2>
class FunctionImpl<R (C, A0, A1, A2)> : public Function
{
public:

    /**
     * \brief Constructor
     */
    FunctionImpl(const std::string& name, boost::function<R (C, A0, A1, A2)> function)
        : Function(name, mapType<R>(), list_of(mapType<A0>())(mapType<A1>())(mapType<A2>()))
        , m_function(function)
    {
    }

protected:

    /**
     * \see Function::execute
     */
    virtual Value execute(const UserObject& object, const Args& args) const
    {
        return CallHelper<R, C>::call(m_function, object.get<C>()
                                      , convertArg<A0>(args, 0, name())
                                      , convertArg<A1>(args, 1, name())
                                      , convertArg<A2>(args, 2, name()));
    }

private:

    boost::function<R (C, A0, A1, A2)> m_function; ///< Object containing the actual function to call
};

/*
 * Specialization of FunctionImpl for functions taking 4 arguments
 */
template <typename C, typename R, typename A0, typename A1, typename A2, typename A3>
class FunctionImpl<R (C, A0, A1, A2, A3)> : public Function
{
public:

    /**
     * \brief Constructor
     */
    FunctionImpl(const std::string& name, boost::function<R (C, A0, A1, A2, A3)> function)
        : Function(name, mapType<R>(), list_of(mapType<A0>())(mapType<A1>())(mapType<A2>())(mapType<A3>()))
        , m_function(function)
    {
    }

protected:

    /**
     * \see Function::execute
     */
    virtual Value execute(const UserObject& object, const Args& args) const
    {
        return CallHelper<R, C>::call(m_function, object.get<C>()
                                      , convertArg<A0>(args, 0, name())
                                      , convertArg<A1>(args, 1, name())
                                      , convertArg<A2>(args, 2, name())
                                      , convertArg<A3>(args, 3, name()));
    }

private:

    boost::function<R (C, A0, A1, A2, A3)> m_function; ///< Object containing the actual function to call
};

/*
 * Specialization of FunctionImpl for functions taking 5 arguments
 */
template <typename C, typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
class FunctionImpl<R (C, A0, A1, A2, A3, A4)> : public Function
{
public:

    /**
     * \brief Constructor
     */
    FunctionImpl(const std::string& name, boost::function<R (C, A0, A1, A2, A3, A4)> function)
        : Function(name, mapType<R>(), list_of(mapType<A0>())(mapType<A1>())(mapType<A2>())(mapType<A3>())(mapType<A4>()))
        , m_function(function)
    {
    }

protected:

    /**
     * \see Function::execute
     */
    virtual Value execute(const UserObject& object, const Args& args) const
    {
        return CallHelper<R, C>::call(m_function, object.get<C>()
                                      , convertArg<A0>(args, 0, name())
                                      , convertArg<A1>(args, 1, name())
                                      , convertArg<A2>(args, 2, name())
                                      , convertArg<A3>(args, 3, name())
                                      , convertArg<A4>(args, 4, name()));
    }

private:

    boost::function<R (C, A0, A1, A2, A3, A4)> m_function; ///< Object containing the actual function to call
};


/*
 * Specialization of FunctionImpl for composed functions taking no argument
 */
template <typename C, typename N, typename M, typename R>
class FunctionImpl<R (N), M (C)> : public Function
{
public:

    /**
     * \brief Constructor
     */
    template <typename F1, typename F2>
    FunctionImpl(const std::string& name, F1 function, F2 accessor)
        : Function(name, mapType<R>())
        , m_function(boost::bind(function, boost::bind(accessor, _1)))
    {
    }

protected:

    /**
     * \see Function::execute
     */
    virtual Value execute(const UserObject& object, const Args&) const
    {
        return CallHelper<R, C>::call(m_function, object.get<C>());
    }

private:

    boost::function<R (C)> m_function; ///< Object containing the actual function to call
};

/*
 * Specialization of FunctionImpl for composed functions taking 1 argument
 */
template <typename C, typename N, typename M, typename R, typename A0>
class FunctionImpl<R (N, A0), M (C)> : public Function
{
public:

    /**
     * \brief Constructor
     */
    template <typename F1, typename F2>
    FunctionImpl(const std::string& name, F1 function, F2 accessor)
        : Function(name, mapType<R>(), list_of(mapType<A0>()))
        , m_function(boost::bind(function, boost::bind(accessor, _1), _2))
    {
    }

protected:

    /**
     * \see Function::execute
     */
    virtual Value execute(const UserObject& object, const Args& args) const
    {
        return CallHelper<R, C>::call(m_function, object.get<C>()
                                      , convertArg<A0>(args, 0, name()));

    }

private:

    boost::function<R (C, A0)> m_function; ///< Object containing the actual function to call
};

/*
 * Specialization of FunctionImpl for composed functions taking 2 arguments
 */
template <typename C, typename N, typename M, typename R, typename A0, typename A1>
class FunctionImpl<R (N, A0, A1), M (C)> : public Function
{
public:

    /**
     * \brief Constructor
     */
    template <typename F1, typename F2>
    FunctionImpl(const std::string& name, F1 function, F2 accessor)
        : Function(name, mapType<R>(), list_of(mapType<A0>())(mapType<A1>()))
        , m_function(boost::bind(function, boost::bind(accessor, _1), _2, _3))
    {
    }

protected:

    /**
     * \see Function::execute
     */
    virtual Value execute(const UserObject& object, const Args& args) const
    {
        return CallHelper<R, C>::call(m_function, object.get<C>()
                                      , convertArg<A0>(args, 0, name())
                                      , convertArg<A1>(args, 1, name()));

    }

private:

    boost::function<R (C, A0, A1)> m_function; ///< Object containing the actual function to call
};

/*
 * Specialization of FunctionImpl for composed functions taking 3 arguments
 */
template <typename C, typename N, typename M, typename R, typename A0, typename A1, typename A2>
class FunctionImpl<R (N, A0, A1, A2), M (C)> : public Function
{
public:

    /**
     * \brief Constructor
     */
    template <typename F1, typename F2>
    FunctionImpl(const std::string& name, F1 function, F2 accessor)
        : Function(name, mapType<R>(), list_of(mapType<A0>())(mapType<A1>())(mapType<A2>()))
        , m_function(boost::bind(function, boost::bind(accessor, _1), _2, _3, _4))
    {
    }

protected:

    /**
     * \see Function::execute
     */
    virtual Value execute(const UserObject& object, const Args& args) const
    {
        return CallHelper<R, C>::call(m_function, object.get<C>()
                                   , convertArg<A0>(args, 0, name())
                                   , convertArg<A1>(args, 1, name())
                                   , convertArg<A2>(args, 2, name()));

    }

private:

    boost::function<R (C, A0, A1, A2)> m_function; ///< Object containing the actual function to call
};

/*
 * Specialization of FunctionImpl for composed functions taking 4 arguments
 */
template <typename C, typename N, typename M, typename R, typename A0, typename A1, typename A2, typename A3>
class FunctionImpl<R (N, A0, A1, A2, A3), M (C)> : public Function
{
public:

    /**
     * \brief Constructor
     */
    template <typename F1, typename F2>
    FunctionImpl(const std::string& name, F1 function, F2 accessor)
        : Function(name, mapType<R>(), list_of(mapType<A0>())(mapType<A1>())(mapType<A2>())(mapType<A3>()))
        , m_function(boost::bind(function, boost::bind(accessor, _1), _2, _3, _4, _5))
    {
    }

protected:

    /**
     * \see Function::execute
     */
    virtual Value execute(const UserObject& object, const Args& args) const
    {
        return CallHelper<R, C>::call(m_function, object.get<C>()
                                      , convertArg<A0>(args, 0, name())
                                      , convertArg<A1>(args, 1, name())
                                      , convertArg<A2>(args, 2, name())
                                      , convertArg<A3>(args, 3, name()));

    }

private:

    boost::function<R (C, A0, A1, A2, A3)> m_function; ///< Object containing the actual function to call
};

/*
 * Specialization of FunctionImpl for composed functions taking 5 arguments
 */
template <typename C, typename N, typename M, typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
class FunctionImpl<R (N, A0, A1, A2, A3, A4), M (C)> : public Function
{
public:

    /**
     * \brief Constructor
     */
    template <typename F1, typename F2>
    FunctionImpl(const std::string& name, F1 function, F2 accessor)
        : Function(name, mapType<R>(), list_of(mapType<A0>())(mapType<A1>())(mapType<A2>())(mapType<A3>())(mapType<A4>()))
        , m_function(boost::bind(function, boost::bind(accessor, _1), _2, _3, _4, _5, _6))
    {
    }

protected:

    /**
     * \see Function::execute
     */
    virtual Value execute(const UserObject& object, const Args& args) const
    {
        return CallHelper<R, C>::call(m_function, object.get<C>()
                                      , convertArg<A0>(args, 0, name())
                                      , convertArg<A1>(args, 1, name())
                                      , convertArg<A2>(args, 2, name())
                                      , convertArg<A3>(args, 3, name())
                                      , convertArg<A4>(args, 4, name()));

    }

private:

    boost::function<R (C, A0, A1, A2, A3, A4)> m_function; ///< Object containing the actual function to call
};

} // namespace detail

} // namespace camp


#endif // CAMP_DETAIL_FUNCTIONIMPL_HPP
