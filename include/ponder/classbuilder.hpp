/****************************************************************************
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Copyright (C) 2015-2018 Nick Trout.
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
#ifndef PONDER_CLASSBUILDER_HPP
#define PONDER_CLASSBUILDER_HPP

#include <ponder/type.hpp>
#include <ponder/classget.hpp>
#include <ponder/class.hpp>
#include <ponder/uses/uses.hpp>
#include <ponder/detail/functionimpl.hpp>
#include <ponder/detail/constructorimpl.hpp>
#include <ponder/detail/propertyfactory.hpp>
#include <ponder/pondertype.hpp>
#include <ponder/userdata.hpp>
#include <cassert>
#include <string>

namespace ponder {
        
/**
 * \brief Proxy class which fills a metaclass with its members
 *
 * This class is returned by Class::declare<T> in order construct a
 * new metaclass. It contains functions to declare and bind metaproperties,
 * metafunctions, base metaclasses, metaconstructors, etc. with many overloads
 * in order to accept as many types of binds as possible.
 *
 * ClassBuilder also contains functions to set attributes of metafunctions
 * and metaproperties.
 *
 * This class should never be explicitely instantiated, unless you
 * need to split the metaclass creation in multiple parts.
 */
template <typename T>
class ClassBuilder
{
public:
    
    /**
     * \brief Construct the builder with a target metaclass to fill
     *
     * \param target Metaclass to build
     */
    ClassBuilder(Class& target);

    /**
     * \brief Declare a base metaclass
     *
     * The template parameter U is the C++ base class of T.
     *
     * This function makes the target metaclass inherit of all the metaproperties and
     * metafunctions of the given base metaclass.
     *
     * \note We *do not* support virtual inheritance fully here due to the associated problems
     *       with compiler specific class layouts. e.g. see Class::applyOffset.
     *
     * \return Reference to this, in order to chain other calls
     *
     * \throw ClassNotFound no metaclass is bound to U
     */
    template <typename U>
    ClassBuilder<T>& base();

    /**
     * \brief Declare a new property from a single accessor
     *
     * The accessor parameter can be a getter of any valid type, or a direct
     * pointer-to-member (which is considered both a getter and a setter)
     *
     * Example:
     *
     * \code
     * struct Point
     * {
     *     float x, y;
     *
     *     float length() const;
     * };
     *
     * ponder::Class::declare<Point>("Point")
     *     .property("x",      &Point::x)       // getter + setter
     *     .property("y",      &Point::y)       // getter + setter
     *     .property("length", &Point::length); // getter only
     * \endcode

     * \param name Name of the property (must be unique within the metaclass)
     * \param accessor Accessor to the C++ implementation of the property
     *
     * \return Reference to this, in order to chain other calls
     */
    template <typename F>
    ClassBuilder<T>& property(IdRef name, F accessor);

    /**
     * \brief Declare a new property from a pair of accessors
     *
     * The accessor1 and accessor2 parameters can be a pair of getter/setter, or
     * two getters which must be composed to form a single getter.
     * If F1 is a direct pointer-to-member, it is considered both a getter and a setter.
     *
     * Having two getters allows to expose a property which requires an extra level of 
     * indirection to be accessed (for example, a property of a member of the class instead of 
     * a property of the class itself).
     *
     * Example:
     *
     * \code
     * struct Point {float x, y;};
     * 
     * class Entity
     * {
     * public:
     *     Point p;
     * };
     *
     * ponder::Class::declare<Entity>("Entity")
     *     .property("x", &Point::x, &Entity::p)  // will internally resolve to e.p.x
     *     .property("y", &Point::y, &Entity::p); // will internally resolve to e.p.y
     * \endcode
     *
     * \param name Name of the property (must be unique within the metaclass)
     * \param accessor1 First accessor to the C++ implementation of the property (getter)
     * \param accessor2 Second accessor to the C++ implementation of the property (setter or 
     *        getter to compose)
     * \return Reference to this, in order to chain other calls
     */
    template <typename F1, typename F2>
    ClassBuilder<T>& property(IdRef name, F1 accessor1, F2 accessor2);

    /**
     * \brief Declare a new function from any bindable type
     *
     * The function parameter can be any valid type: a non-member function,
     * member function, const, non-const, lambda, etc. Polices can be applied to the
     * function to affect things like the way objects are returned. See \ref ponder::policy.
     *
     * \param name Name of the function (must be unique within the metaclass)
     * \param function C++ callable entity to bind to the function
     * \param policies Optional policies applied to function exposer
     * \return Reference to this, in order to chain other calls
     *
     * \sa property(), ponder::policy, \ref eg_page_shapes
     */
    template <typename F, typename... P>
    ClassBuilder<T>& function(IdRef name, F function, P... policies);

    /**
     * \brief Declare a constructor for the metaclass.
     * 
     * Variable number of parameters can be passed.
     *
     * \return Reference to this, in order to chain other calls
     */
    template <typename... A>
    ClassBuilder<T>& constructor();
    
    /**
     * \brief Add properties and/or functions from an external source
     *
     * The purpose of this function is to allow the binding of classes
     * that already use a similar system of metaproperties and metafunctions,
     * with a direct mapping from external attributes to Ponder ones.
     *
     * The mapping process must be done in a specific mapper class (see below), thus avoiding
     * to manually write the mapping for every class.
     *
     * The mapper class must accept a template parameter (which is the target C++ class)
     * and be compatible with the following interface:
     *
     * \code
     * template <typename T>
     * class MyClassMapper
     * {
     * public:
     *     MyClassMapper();
     *
     *     size_t propertyCount();
     *     ponder::Property* property(size_t index);
     *
     *     size_t functionCount();
     *     ponder::Function* function(size_t index);
     * };
     * \endcode
     *
     * Example of usage:
     *
     * \code
     * ponder::Class::declare<MyClass>("MyClass")
     *     .external<MyClassMapper>()
     *     ...
     * \endcode
     *
     * \return Reference to this, in order to chain other calls
     */
    template <template <typename> class U>
    ClassBuilder<T>& external();

    /**
     * \brief Add user data to the last declared member type
     *
     * \code
     * ponder::Class::declare<MyClass>("MyClass")
     *     .function("foo", &MyClass::foo)( ponder::UserData("user", 3) );
     * \endcode
     *
     * \return Reference to this, in order to chain other calls
     */
    template <typename... U>
    ClassBuilder<T>& operator () (U&&... uds)
    {
        const std::initializer_list<UserData> il = {uds...};
        for (UserData const& ud : il)
            userDataStore()->setValue(*m_currentType, ud.getName(), ud.getValue());
        return *this;
    }

private:

    ClassBuilder<T>& addProperty(Property* property);
    ClassBuilder<T>& addFunction(Function* function);

    Class* m_target; // Target metaclass to fill
    Type* m_currentType; // Last member type which has been declared
};

} // namespace ponder

#include <ponder/classbuilder.inl>

#endif // PONDER_CLASSBUILDER_HPP
