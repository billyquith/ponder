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


#ifndef CAMP_DETAIL_CONSTRUCTORIMPL_HPP
#define CAMP_DETAIL_CONSTRUCTORIMPL_HPP


#include <camp/constructor.hpp>
#include <camp/valuemapper.hpp>
#include <camp/value.hpp>
#include <camp/valuevisitor.hpp>
#include <camp/errors.hpp>


namespace camp
{
namespace detail
{
/**
 * \brief Helper function which converts an argument to a C++ type
 *
 * The main purpose of this function is to convert any BadType error to
 * a BadArgument one.
 *
 * \param args List of arguments
 * \param index Index of the argument to convert
 *
 * \return Value of args[index] converted to T
 *
 * \thrown BadArgument conversion triggered a BadType error
 */
template <typename T>
inline typename boost::remove_reference<T>::type convertArg(const Args& args, std::size_t index)
{
    try
    {
        return args[index].to<typename boost::remove_reference<T>::type>();
    }
    catch (BadType&)
    {
        CAMP_ERROR(BadArgument(args[index].type(), mapType<T>(), index, "constructor"));
    }
}

/**
 * \brief Check if a value is compatible with a C++ type
 *
 * This is a strong test, we check if the type of the value is directly mappable to T, not
 * just convertible.
 *
 * \param value Value to check
 *
 * \return True if the value is compatible with the type T
 */
template <typename T>
bool checkArg(const Value& value);

/**
 * \brief Implementation of metaconstructors with no parameter
 */
template <typename T>
class ConstructorImpl0 : public Constructor
{
public:

    /**
     * \see Constructor::matches
     */
    virtual bool matches(const Args& args) const
    {
        return (args.count() == 0);
    }

    /**
     * \see Constructor::create
     */
    virtual void* create(const Args&) const
    {
        return new T();
    }
};

/**
 * \brief Implementation of metaconstructors with 1 parameter
 */
template <typename T, typename A0>
class ConstructorImpl1 : public Constructor
{
public:

    /**
     * \see Constructor::matches
     */
    virtual bool matches(const Args& args) const
    {
        return (args.count() == 1)
               && checkArg<A0>(args[0]);
    }

    /**
     * \see Constructor::create
     */
    virtual void* create(const Args& args) const
    {
        return new T(convertArg<A0>(args, 0));
    }
};

/**
 * \brief Implementation of metaconstructors with 2 parameters
 */
template <typename T, typename A0, typename A1>
class ConstructorImpl2 : public Constructor
{
public:

    /**
     * \see Constructor::matches
     */
    virtual bool matches(const Args& args) const
    {
        return (args.count() == 2)
               && checkArg<A0>(args[0])
               && checkArg<A1>(args[1]);
    }

    /**
     * \see Constructor::create
     */
    virtual void* create(const Args& args) const
    {
        return new T(convertArg<A0>(args, 0),
                     convertArg<A1>(args, 1));
    }
};

/**
 * \brief Implementation of metaconstructors with 3 parameters
 */
template <typename T, typename A0, typename A1, typename A2>
class ConstructorImpl3 : public Constructor
{
public:

    /**
     * \see Constructor::matches
     */
    virtual bool matches(const Args& args) const
    {
        return (args.count() == 3)
               && checkArg<A0>(args[0])
               && checkArg<A1>(args[1])
               && checkArg<A2>(args[2]);
    }

    /**
     * \see Constructor::create
     */
    virtual void* create(const Args& args) const
    {
        return new T(convertArg<A0>(args, 0),
                     convertArg<A1>(args, 1),
                     convertArg<A2>(args, 2));
    }
};

/**
 * \brief Implementation of metaconstructors with 4 parameters
 */
template <typename T, typename A0, typename A1, typename A2, typename A3>
class ConstructorImpl4 : public Constructor
{
public:

    /**
     * \see Constructor::matches
     */
    virtual bool matches(const Args& args) const
    {
        return (args.count() == 4)
               && checkArg<A0>(args[0])
               && checkArg<A1>(args[1])
               && checkArg<A2>(args[2])
               && checkArg<A3>(args[3]);
    }

    /**
     * \see Constructor::create
     */
    virtual void* create(const Args& args) const
    {
        return new T(convertArg<A0>(args, 0),
                     convertArg<A1>(args, 1),
                     convertArg<A2>(args, 2),
                     convertArg<A3>(args, 3));
    }
};

/**
 * \brief Implementation of metaconstructors with 5 parameters
 */
template <typename T, typename A0, typename A1, typename A2, typename A3, typename A4>
class ConstructorImpl5 : public Constructor
{
public:

    /**
     * \see Constructor::matches
     */
    virtual bool matches(const Args& args) const
    {
        return (args.count() == 5)
               && checkArg<A0>(args[0])
               && checkArg<A1>(args[1])
               && checkArg<A2>(args[2])
               && checkArg<A3>(args[3])
               && checkArg<A4>(args[4]);
    }

    /**
     * \see Constructor::create
     */
    virtual void* create(const Args& args) const
    {
        return new T(convertArg<A0>(args, 0),
                     convertArg<A1>(args, 1),
                     convertArg<A2>(args, 2),
                     convertArg<A3>(args, 3),
                     convertArg<A4>(args, 4));
    }
};

/**
 * \brief Value visitor which checks the type of the visited value against the C++ type T
 */
template <typename T>
struct CheckTypeVisitor : public ValueVisitor<bool>
{
    /**
     * \brief Common case: check mapping
     */
    template <typename U>
    bool operator()(const U&)
    {
        return mapType<T>() == mapType<U>();
    }

    /**
     * \brief Special case of enum objects: check metaenum and bound type
     */
    bool operator()(const EnumObject& obj)
    {
        const Enum* targetEnum = enumByTypeSafe<T>();
        return targetEnum && (*targetEnum == obj.getEnum());
    }

    /**
     * \brief Special case of user objects: check metaclass and bound type
     */
    bool operator()(const UserObject& obj)
    {
        const Class* targetClass = classByTypeSafe<T>();
        return targetClass && (*targetClass == obj.getClass());
    }
};

template <typename T>
bool checkArg(const Value& value)
{
    return value.visit(CheckTypeVisitor<T>());
}

} // namespace detail

} // namespace camp


#endif // CAMP_DETAIL_CONSTRUCTORIMPL_HPP
