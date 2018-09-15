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
#ifndef PONDER_CLASS_HPP
#define PONDER_CLASS_HPP

#include <ponder/classget.hpp>
#include <ponder/classcast.hpp>
#include <ponder/property.hpp>
#include <ponder/function.hpp>
#include <ponder/userobject.hpp>
#include <ponder/detail/typeid.hpp>
#include <ponder/detail/dictionary.hpp>
#include <string>
#include <map>

namespace ponder {
    
template <typename T> class ClassBuilder;
class Constructor;
class Args;
class ClassVisitor;
  
/**
 * \brief ponder::Class represents a metaclass composed of properties and functions
 *
 * ponder::Class is the main class of the Ponder API. It defines a metaclass, which
 * is an abstract representation of a C++ class with its own properties,
 * functions, constructors, base classes, etc.
 *
 * Classes are declared, bound to a C++ type and filled with the Class::declare()
 * function.
 *
 * \snippet simple.cpp eg_simple_class
 *
 * \snippet simple.cpp eg_simple_declare
 *
 * It then provides a set of accessors to retrieve its member functions and
 * properties. See Class::function() and Class::property().
 *
 * Another way to inspect a class, which is more type-safe, is to use a ClassVisitor.
 *
 * \code
 * MyVisitor visitor;
 * metaclass.visit(visitor);
 * \endcode
 *
 * It also allows to create and destroy instances of the bound C++ class.
 *
 * \snippet simple.cpp eg_simple_create
 *
 * \remark All function and property names are unique within the metaclass.
 *
 * \sa ClassBuilder, Function, Property, Enum
 */
class PONDER_API Class : public Type
{    
    PONDER__NON_COPYABLE(Class);
    
    // Structure holding informations about a base metaclass
    struct BaseInfo
    {
        const Class* base;
        int offset;
    };
    
    // These are shared_ptr as the objects can be inherited. When this happens the
    // pointers are copied.
    typedef std::shared_ptr<Constructor> ConstructorPtr;
    typedef std::shared_ptr<Property> PropertyPtr;
    typedef std::shared_ptr<Function> FunctionPtr;
    
    typedef std::vector<BaseInfo> BaseList;
    typedef std::vector<ConstructorPtr> ConstructorList;
    typedef detail::Dictionary<Id, IdRef, PropertyPtr> PropertyTable;
    typedef detail::Dictionary<Id, IdRef, FunctionPtr> FunctionTable;
    typedef void (*Destructor)(const UserObject&, bool);
    typedef UserObject (*UserObjectCreator)(void*);
    
    std::size_t m_sizeof;           // Size of the class in bytes.
    Id m_id;                        // Name of the metaclass
    FunctionTable m_functions;      // Table of metafunctions indexed by ID
    PropertyTable m_properties;     // Table of metaproperties indexed by ID
    BaseList m_bases;               // List of base metaclasses
    ConstructorList m_constructors; // List of metaconstructors
    Destructor m_destructor;        // Destructor (function able to delete an abstract object)
    UserObjectCreator m_userObjectCreator; // Convert pointer of class instance to UserObject

public:     // declaration

    /**
     * \brief Declare a new metaclass
     *
     * Call this to create a new metaclass. The template parameter T is the
     * C++ class that will be bound to the metaclass.
     *
     * \param id Name of the metaclass in Ponder. This name identifies
     *           the metaclass and thus has to be unique. If not specified, the C++ type
     *           id is used.
     * \return A ClassBuilder<T> object that will provide functions
     *         to fill the new metaclass with properties, functions, etc.
     *
     * \remark It is best to leave the name blank and use the default class name.
     */
    template <typename T>
    static ClassBuilder<T> declare(IdRef id = ponder::Id());

    /**
     * \brief Undeclare an existing metaclass
     *
     * Use this to undeclare a metaclass that you no longer require. E.g. from a dynamically
     * loaded library that is being unloaded.
     *
     * \note Do *not* use automatic metaclass declaration (PONDER_AUTO_TYPE) for the class 
     *       or it will keep being recreated by Ponder.
     *
     * \param id Name identifier of a previously declared class
     *
     * \see Class::declare, Enum::undeclare
     */
    template <typename T>
    static void undeclare(IdRef id = ponder::Id());

public:     // reflection

    /**
     * \brief Return the name of the metaclass
     *
     * \return String containing the name of the metaclass
     */
    IdReturn name() const;

    /**
     * \brief Return the total number of base metaclasses of this metaclass
     *
     * \return Number of base metaclasses
     */
    std::size_t baseCount() const;
    
    /**
     * \brief Return a base metaclass from its index
     *
     * \param index Index of the base to get
     * \return Reference to the index-th base metaclass of this metaclass
     *
     * \throw OutOfRange index is out of range
     */
    const Class& base(std::size_t index) const;

    /**
     * \brief Return the total number of constructors of this metaclass
     *
     * \return Number of constructors
     */
    std::size_t constructorCount() const;
    
    /**
     * \brief Access constructors by index
     *
     * \param index Index
     *
     * \return Constructor
     */
    const Constructor* constructor(std::size_t index) const;
    
    /**
     * \brief Destroy a UserObject instance
     *
     * \param uobj User object to destruct
     * \param destruct True for destruct (placement new), else destroy (new)
     */
    void destruct(const UserObject &uobj, bool destruct) const;
    
    /**
     * \brief Return the total number of functions of this metaclass
     *
     * \return Number of functions
     */
    std::size_t functionCount() const;

    /**
     * \brief Check if this metaclass contains the given function
     *
     * \param name Name of the function to check
     *
     * \return True if the function is in the metaclass, false otherwise
     */
    bool hasFunction(IdRef name) const;

    /**
     * \brief Get a function from its index in this metaclass
     *
     * \param index Index of the function to get
     *
     * \return Reference to the function
     *
     * \throw OutOfRange index is out of range
     */
    const Function& function(std::size_t index) const;

    /**
     * \brief Get a function from its name
     *
     * \param name Name of the function to get (case sensitive)
     *
     * \return Reference to the function
     *
     * \throw FunctionNotFound \a name is not a function of the metaclass
     */
    const Function& function(IdRef name) const;

    /**
     * \brief Get a function iterator
     *
     * \return An iterator that can be used to iterator over all functions
     *
     * \code
     * for (auto&& func : classByType<MyClass>().functionIterator())
     *     foo(func.name(), func.value());
     * \endcode
     */
    FunctionTable::Iterator functionIterator() const;

    /**
     * \brief Look up a function by name and return success
     *
     * \param name Name of the function to get (case sensitive)
     * \param funcRet Function returned, if return was true
     * \return Boolean. True if function found, else if not, false
     *
     * \code
     * const Function *func;
     * if (classByType<MyClass>().tryFunction("foo", func))
     *     func.call(...);
     * \endcode
     */
    bool tryFunction(const IdRef name, const Function*& funcRet) const;

    /**
     * \brief Return the total number of properties of this metaclass
     *
     * \return Number of properties
     */
    std::size_t propertyCount() const;

    /**
     * \brief Check if this metaclass contains the given property
     *
     * \param name Name of the property to check
     * \return True if the property is in the metaclass, false otherwise
     */
    bool hasProperty(IdRef name) const;

    /**
     * \brief Get a property from its index in this metaclass
     *
     * \param index Index of the property to get
     * \return Reference to the property
     *
     * \throw OutOfRange index is out of range
     */
    const Property& property(std::size_t index) const;

    /**
     * \brief Get a property from its name
     *
     * \param name Name of the property to get (case sensitive)
     * \return Reference to the property
     *
     * \throw PropertyNotFound \a name is not a property of the metaclass
     */
    const Property& property(IdRef name) const;
    
    /**
     * \brief Get a property iterator
     *
     * \return An iterator that can be used to iterator over all properties
     *
     * \code
     * for (auto&& prop : ponder::classByType<MyClass>().propertyIterator())
     *     foo(prop.name(), prop.value());
     * \endcode
     */
    PropertyTable::Iterator propertyIterator() const;
    
    /**
     * \brief Look up a property by name and return success
     *
     * \param name Name of the property to get (case sensitive)
     * \param propRet Property returned, if return was true
     * \return Boolean. True if property found, else if not, false
     *
     * \code
     * const Property *prop;
     * if (classByType<MyClass>().tryProperty("bar", prop))
     *     ... ;
     * \endcode
     */
    bool tryProperty(const IdRef name, const Property*& propRet) const;
    
    /**
     * \brief Return the memory size of a class instance
     *
     * \return Size in bytes
     */
    std::size_t sizeOf() const;

    /**
     * \brief Create a UserObject from an opaque user pointer
     *
     * \return A UserObject with this class's type
     *
     * \note This relies on the user choosing the correct metaclass. There are no
     *       checks (as the data is opaque).
     *
     * \code
     * auto const& metacls = ponder::classByType<MyClass>();
     * void *ptr = &object;
     * ponder::UserObject uo( metacls.getUserObjectFromPointer(ptr) );
     * \endcode
     */
    UserObject getUserObjectFromPointer(void* ptr) const;

    /**
     * \brief Start visitation of a class
     *
     * \param visitor Visitor to use for visitation
     */
    void visit(ClassVisitor& visitor) const;

    /**
     * \brief Convert a pointer to an object compatible with a base or derived metaclass
     *
     * The target metaclass may be a base or a derived of this, both cases are properly handled.
     *
     * \note Because virtual inheritance implementation is compiler specific this method is 
     *       unreliable where virtual inheritance is used.
     *
     * \param pointer Pointer to convert
     * \param target Target metaclass to convert to
     * \return Converted pointer
     *
     * \throw ClassUnrelated \a target is not a base nor a derived class of this
     */
    void* applyOffset(void* pointer, const Class& target) const;

    /**
     * \brief Operator == to check equality between two metaclasses
     *
     * Two metaclasses are equal if their name is the same.
     *
     * \param other Metaclass to compare with this
     *
     * \return True if both metaclasses are the same, false otherwise
     */
    bool operator == (const Class& other) const;

    /**
     * \brief Operator != to check inequality between two metaclasses
     *
     * \param other Metaclass to compare with this
     *
     * \return True if metaclasses are different, false if they are equal
     */
     bool operator != (const Class& other) const;

private:

    template <typename T> friend class ClassBuilder;
    friend class detail::ClassManager;

    /**
     * \brief Construct the metaclass from its name
     *
     * \param name Name of the metaclass
     */
    Class(IdRef name);

    /**
     * \brief Get the offset of a base metaclass
     *
     * \param base Base metaclass to check
     *
     * \return offset between this and base, or -1 if both classes are unrelated
     */
    int baseOffset(const Class& base) const;
    
};

} // namespace ponder

#include <ponder/class.inl>

#endif // PONDER_CLASS_HPP
