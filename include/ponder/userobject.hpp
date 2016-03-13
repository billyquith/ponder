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


#ifndef PONDER_USEROBJECT_HPP
#define PONDER_USEROBJECT_HPP


#include <ponder/args.hpp>
#include <ponder/classcast.hpp>
#include <ponder/errors.hpp>
#include <ponder/detail/objecttraits.hpp>
#include <ponder/detail/objectholder.hpp>
#include <ponder/detail/util.hpp>
#include <string>


namespace ponder
{
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
     * \brief Construct the user object from an instance
     *
     * This constructor is equivalent to calling UserObject::ref(object),
     * i.e. the object is stored by reference.
     *
     * \param object Instance to store in the user object
     */
    template <typename T>
    UserObject(const T& object);

    /**
     * \brief Construct the user object from a parent object and a member property
     *
     * This constructor allows to create user objects that are accessed through a property
     * of another user object, and that cannot be stored in memory.
     *
     * \param parent Parent user object
     * \param member Member of \a parent used to access the actual object
     */
    UserObject(const UserObject& parent, const UserProperty& member);

    /**
     * \brief Copy constructor
     *
     * \param copy Instance to copy
     */
    UserObject(const UserObject& copy);

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
    typename detail::ObjectTraits<T>::RefReturnType get() const;

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
     * \brief Retrieve the metaclass of the stored instance
     *
     * \return Reference to the instance's metaclass
     *
     * \throw NullObject the stored object has no metaclass
     */
    const Class& getClass() const;

    /**
     * \brief Get the value of an object's property
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
    Value get(const std::string& property) const;

    /**
     * \brief Set the value of an object's property
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
    void set(const std::string& property, const Value& value) const;

    /**
     * \brief Call an object's function
     *
     * This function is defined for convenience, it is a shortcut
     * for `object.getClass().function(name).call(object, args);`
     *
     * \param function Name of the function to call
     * \param args Arguments to pass to the function
     *
     * \return Value returned by the function
     *
     * \throw FunctionNotFound \a function is not a function of the object class
     * \throw ForbiddenCall \a function is not callable
     * \throw NotEnoughArguments too few arguments are provided
     * \throw BadArgument one of the arguments can't be converted to the requested type
     */
    Value call(const std::string& function, const Args& args = Args::empty) const;

    /**
     * \brief Assignment operator
     *
     * \param other User object to assign
     *
     * \return Reference to this
     */
    UserObject& operator = (const UserObject& other);

    /**
     * \brief Operator == to compare equality between two user objects
     *
     * Two user objects are equal if their metaclasses and pointers are both equal
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

public:

    /**
     * \brief Special UserObject instance representing an empty object
     */
    static const UserObject nothing;

    /**
     * \brief Construct a user object from a reference to an object
     *
     * This functions is equivalent to calling UserObject(object).
     *
     * \param object Instance to store in the user object
     *
     * \return UserObject containing a reference to \a  object
     */
    template <typename T>
    static UserObject ref(T& object);

    /**
     * \brief Construct a user object from a const reference to an object
     *
     * This functions is NOT equivalent to calling UserObject(object).
     *
     * \param object Instance to store in the user object
     *
     * \return UserObject containing a const reference to \a object
     */
    template <typename T>
    static UserObject ref(const T& object);

    /**
     * \brief Construct a user object with a copy of an object
     *
     * This functions is NOT equivalent to calling UserObject(object).
     *
     * \param object Instance to store in the user object
     *
     * \return UserObject containing a copy of \a object
     */
    template <typename T>
    static UserObject copy(const T& object);

private:

    friend class Property;

    /**
     * \brief Assign a new value to a property of the object
     *
     * This functions is meant to be used only by Property, and
     * is needed for proper propagation of the modification to the
     * parent objects.
     *
     * \param property Property to modify
     * \param value New value to assign
     */
    void set(const Property& property, const Value& value) const;

    /**
     * \brief Recursively set a property
     *
     * See the description of UserObject::set(property, value).
     *
     * \param object Object to modify
     * \param property Property to set when the recursion is over
     * \param value Value to assign when the recursion is over
     */
    void cascadeSet(const UserObject& object, const Property& property, const Value& value) const;

private:

    const Class* m_class; ///< Metaclass of the stored object
    std::shared_ptr<detail::AbstractObjectHolder> m_holder; ///< Optional abstract holder storing the object
    std::unique_ptr<ParentObject> m_parent; ///< Optional parent object
    const UserObject* m_child; ///< Optional pointer to the child object (m_parent.object.m_child == this)
};


/**
 * \brief Structure holding the informations about a parent object
 *
 * A parent object is composed of a <object, property> pair.
 */
class ParentObject : detail::noncopyable
{
public:

    /**
     * \brief Default constructor
     */
    ParentObject(const UserObject& obj, const UserProperty& mem) : object(obj), member(mem) {}

    UserObject object; ///< Parent object
    const UserProperty& member; ///< Member of the parent giving access to the child object
    UserObject lastValue; ///< Last value of the object (stored here for persistency)
};


} // namespace ponder

#include <ponder/userobject.inl>


#endif // PONDER_USEROBJECT_HPP
