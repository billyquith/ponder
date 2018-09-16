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
#ifndef PONDER_USEROBJECT_HPP
#define PONDER_USEROBJECT_HPP

#include <ponder/args.hpp>
#include <ponder/classcast.hpp>
#include <ponder/errors.hpp>
#include <ponder/detail/objecttraits.hpp>
#include <ponder/detail/objectholder.hpp>
#include <ponder/detail/util.hpp>

namespace ponder {
    
class Property;
class UserProperty;
class Value;
class Args;
class ParentObject;

/**
 * \brief Wrapper to manipulate user objects in the Ponder system
 *
 * ponder::UserObject is an abstract representation of object instances, which can safely
 * be passed to and manipulated by all the entities in Ponder.
 *
 * \note UserObjects are stored interally as objects (a copy) or references (an existing 
 *       object). To be sure which you are constructing use UserObject::makeRef() or
 *       UserObject::makeCopy().
 *
 * \sa EnumObject
 */
class PONDER_API UserObject
{
public:

    /**
     * \brief Default constructor
     *
     * Constructs an empty/invalid object
     */
    UserObject();

    /**
     * \brief Construct the user object from an instance copy
     *
     * \param object Instance to store in the user object
     *
     * \sa makeRef(), makeCopy()
     */
    template <typename T>
    UserObject(const T& object);

    /**
     * \brief Construct the user object from an instance reference
     *
     * \param object Pointer to the object to reference in the user object
     *
     * \sa makeRef(), makeCopy()
     */
    template <typename T>
    UserObject(T* object);

    /**
     * \brief Construct a user object from a reference to an object
     *
     * This functions is equivalent to calling `UserObject(&object)`.
     *
     * \param object Instance to store in the user object
     * \return UserObject containing a reference to \a object
     */
    template <typename T>
    static UserObject makeRef(T& object);

    /**
     * \brief Construct a user object from a const reference to an object
     *
     * This functions is equivalent to calling `UserObject(&object)`.
     *
     * \param object Instance to store in the user object
     * \return UserObject containing a const reference to \a object
     */
    template <typename T>
    static UserObject makeRef(T* object);

    /**
     * \brief Construct a user object with a copy of an object
     *
     * This functions is *not* equivalent to calling UserObject(object).
     *
     * \param object Instance to store in the user object
     *
     * \return UserObject containing a copy of \a object
     */
    template <typename T>
    static UserObject makeCopy(const T& object);

    /**
     * \brief Copy constructor
     *
     * \param other instance to copy
     */
    UserObject(const UserObject& other);

    /**
     * \brief Move constructor
     *
     * \param other instance to move
     */
    UserObject(UserObject&& other) noexcept;
    
    /**
     * \brief Copy assignment operator
     *
     * \param other User object to assign
     *
     * \return Reference to this
     */
    UserObject& operator = (const UserObject& other);

    /**
     * \brief Move assignment operator
     *
     * \param other User object to assign
     *
     * \return Reference to this
     */
    UserObject& operator = (UserObject&& other) noexcept;

    /**
     * \brief Retrieve the instance stored in the user object
     *
     * The template parameter T is the type to convert the instance to.
     * T must be compatible with the original type of the instance.
     *
     * \return Reference to the instance of the stored object
     *
     * \throw NullObject the stored object is invalid
     * \throw ClassNotFound T has not metaclass
     * \throw ClassUnrelated the type of the object is not compatible with T
     */
    template <typename T>
    typename detail::ReferenceTraits<T>::ReferenceType get() const;

    /**
     * \brief Retrieve the address of the stored object
     *
     * This function must be used with caution, as the returned address
     * may not be what you expect it to be!
     *
     * \return Pointer to the stored object
     */
    void* pointer() const;
    
    /**
     * \brief Get a const reference to the object data contained
     *
     * Returns a const reference to the contained object, of type T. The user is responsible for
     * ensuring that the type passed is correct. See ref() for non-const ref.
     *
     * \return A const reference to the contained object.
     */
    template <typename T>
    const T& cref() const;

   /**
    * \brief Get a non-const reference to the object data contained
    *
    * Returns a reference to the contained object, of type T. The user is responsible for
    * ensuring that the type passed is correct. See cref() for const ref.
    *
    * \return A reference to the contained object.
    */
    template <typename T>
    T& ref() const;

    /**
     * \brief Retrieve the metaclass of the stored instance
     *
     * \return Reference to the instance's metaclass
     *
     * \throw NullObject the stored object has no metaclass
     */
    const Class& getClass() const;

    /**
     * \brief Get the value of an object's property by name
     *
     * This function is defined for convenience, it is a shortcut
     * for `object.getClass().property(name).get(object);`
     *
     * \param property Name of the property to get
     *
     * \return Current value of the property
     *
     * \throw PropertyNotFound \a property is not a property of the object
     * \throw ForbiddenRead \a property is not readable
     */
    Value get(IdRef property) const;

    /**
     * \brief Get the value of an object's property by index
     *
     * This function is defined for convenience, it is a shortcut
     * for `object.getClass().property(index).get(object);`
     *
     * \param index Index of the property to get
     *
     * \return Current value of the property
     *
     * \throw OutOfRange index is invalid
     * \throw ForbiddenRead \a property is not readable
     */
    Value get(std::size_t index) const;

    /**
     * \brief Set the value of an object's property by name
     *
     * This function is defined for convenience, it is a shortcut
     * for `object.getClass().property(name).set(object, value);`
     *
     * \param property Name of the property to set
     * \param value Value to set
     *
     * \throw PropertyNotFound \a property is not a property of the object
     * \throw ForbiddenWrite \a property is not writable
     * \throw BadType \a value can't be converted to the property's type
     */
    void set(IdRef property, const Value& value) const;

    /**
     * \brief Set the value of an object's property by index
     *
     * This function is defined for convenience, it is a shortcut
     * for `object.getClass().property(index).set(object, value);`
     *
     * \param index Index of the property to set
     * \param value Value to set
     *
     * \throw OutOfRange index is invalid
     * \throw ForbiddenWrite \a property is not writable
     * \throw BadType \a value can't be converted to the property's type
     */
    void set(std::size_t index, const Value& value) const;

    /**
     * \brief Operator == to compare equality between two user objects
     *
     * Two user objects are equal if their metaclasses and pointers are both equal, 
     * i.e. they point to the same object, not if the object *values* are the same.
     *
     * \param other User object to compare with this
     *
     * \return True if both user objects are the same, false otherwise
     */
    bool operator == (const UserObject& other) const;

    /**
     * \brief Operator != to compare inequality between two user objects
     *
     * \see operator ==
     */
    bool operator != (const UserObject& other) const {return !(*this == other);}

    /**
     * \brief Operator < to compare two user objects
     *
     * \param other User object to compare with this
     *
     * \return True if this < other
     */
    bool operator < (const UserObject& other) const;

    /**
     * \brief Special UserObject instance representing an empty object
     */
    static const UserObject nothing;

private:

    friend class Property;

    /**
     * \brief Assign a new value to a property of the object
     *
     * \param property Property to modify
     * \param value New value to assign
     */
    void set(const Property& property, const Value& value) const;
    
    UserObject(const Class* cls, detail::AbstractObjectHolder* h)
        :   m_class(cls)
        ,   m_holder(h)
    {}

    /// Metaclass of the stored object
    const Class* m_class;
    
    /// Optional abstract holder storing the object
    std::shared_ptr<detail::AbstractObjectHolder> m_holder;
};

} // namespace ponder

#include <ponder/userobject.inl>

#endif // PONDER_USEROBJECT_HPP
