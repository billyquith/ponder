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


#ifndef PONDER_DETAIL_CONSTRUCTORIMPL_HPP
#define PONDER_DETAIL_CONSTRUCTORIMPL_HPP


#include <ponder/constructor.hpp>
#include <ponder/valuemapper.hpp>
#include <ponder/value.hpp>
#include <ponder/valuevisitor.hpp>
#include <ponder/userobject.hpp>


namespace ponder
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
inline typename std::remove_reference<T>::type convertArg(const Args& args, std::size_t index)
{
    try
    {
        return args[index].to<typename std::remove_reference<T>::type>();
    }
    catch (const BadType&)
    {
        PONDER_ERROR(BadArgument(args[index].type(), mapType<T>(), index, "constructor"));
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
 * \brief Implementation of metaconstructors with 1 parameter
 */
template <typename T, typename... A>
class ConstructorImpl : public Constructor
{
    template <typename... As, std::size_t... Is>
    static inline bool checkArgs(const Args& args, index_sequence<Is...>)
    {
        return allTrue(checkArg<As>(args[Is])...);
    }

    template <typename... As, std::size_t... Is>
    static inline UserObject createWithArgs(const Args& args, index_sequence<Is...>)
    {
        return new T(convertArg<As>(args, Is)...);
    }

public:

    /**
     * \see Constructor::matches
     */
    virtual bool matches(const Args& args) const override
    {
        return args.count() == sizeof...(A) && checkArgs<A...>(args, make_index_sequence<sizeof...(A)>());
    }

    /**
     * \see Constructor::create
     */
    virtual UserObject create(const Args& args) const override
    {
        return createWithArgs<A...>(args, make_index_sequence<sizeof...(A)>());
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

} // namespace ponder


#endif // PONDER_DETAIL_CONSTRUCTORIMPL_HPP
