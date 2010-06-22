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


#ifndef CAMP_CAMPTYPE_HPP
#define CAMP_CAMPTYPE_HPP


#include <camp/config.hpp>


namespace camp
{
namespace detail
{
template <typename T> struct StaticTypeId;
template <typename T> const char* staticTypeId(const T&);
CAMP_API void ensureTypeRegistered(const char* id, void (*registerFunc)());
}

/**
 * \brief Macro used to register a C++ type to CAMP
 *
 * Every type manipulated by CAMP must be registered with CAMP_TYPE, CAMP_AUTO_TYPE or their NONCOPYABLE versions.
 *
 * Example:
 *
 * \code
 * class MyClass
 * {
 *     class MyNestedClass
 *     {
 *     };
 * };
 * CAMP_TYPE(MyClass)
 * CAMP_TYPE(MyClass::MyNestedClass)
 * \endcode
 */
#define CAMP_TYPE(type) \
    namespace camp \
    { \
        namespace detail \
        { \
            template <> struct StaticTypeId<type> \
            { \
                static const char* get(bool = true) {return #type;} \
                enum {defined = true, copyable = true}; \
            }; \
        } \
    }

/**
 * \brief Macro used to register a C++ type to CAMP with automatic metaclass creation
 *
 * Using this macro rather than CAMP_TYPE will make CAMP automatically call
 * the provided registration function the first time the metaclass is requested.
 * This is useful when you don't want to have to manually call an "init" function to
 * create your metaclass.
 *
 * Every type manipulated by CAMP must be registered with CAMP_TYPE, CAMP_AUTO_TYPE or their NONCOPYABLE versions.
 *
 * Example:
 *
 * \code
 * class MyClass
 * {
 * public:
 *
 *     static void registerMetaClass();
 * };
 * CAMP_AUTO_TYPE(MyClass, &MyClass::registerMetaClass)
 *
 * void MyClass::registerMetaClass()
 * {
 *     camp::Class::declare<MyClass>("MyClass")
 *         ...;
 * }
 * \endcode
 *
 * \sa CAMP_TYPE
 */
#define CAMP_AUTO_TYPE(type, registerFunc) \
    namespace camp \
    { \
        namespace detail \
        { \
            template <> struct StaticTypeId<type> \
            { \
                static const char* get(bool checkRegister = true) \
                { \
                    if (checkRegister) \
                        detail::ensureTypeRegistered(#type, registerFunc); \
                    return #type; \
                } \
                enum {defined = true, copyable = true}; \
            }; \
        } \
    }

/**
 * \brief Macro used to register a non-copyable C++ type to CAMP
 *
 * Disabled copy and assignment cannot be detected at compile-time, thus users have to explicitely tell CAMP
 * when a type is not copyable/assignable. Objects of a non-copyable class can be modified through their metaproperties,
 * but they can't be written with a single call to replace to whole object.
 *
 * Every type manipulated by CAMP must be registered with CAMP_TYPE, CAMP_AUTO_TYPE or their NONCOPYABLE versions.
 *
 * Example:
 *
 * \code
 * class NonCopyable : boost::noncopyable
 * {
 *     int x;
 * };
 * CAMP_TYPE_NONCOPYABLE(NonCopyable)
 *
 * class MyClass
 * {
 *     NonCopyable* nc;
 * };
 * CAMP_TYPE(MyClass)
 *
 * MyClass c;
 * const camp::Class& m1 = camp::classByObject(c);
 * const camp::Class& m2 = camp::classByObject(c.nc);
 * const camp::Property& p1 = m1.property("nc");
 * const camp::Property& p2 = m2.property("x");
 * p1.set(c, NonCopyable()); // ERROR
 * p2.set(p1.get(c).to<camp::UserObject>(), 10); // OK
 * \endcode
 *
 * \sa CAMP_TYPE
 */
#define CAMP_TYPE_NONCOPYABLE(type) \
    namespace camp \
    { \
        namespace detail \
        { \
            template <> struct StaticTypeId<type> \
            { \
                static const char* get(bool = true) {return #type;} \
                enum {defined = true, copyable = false}; \
            }; \
        } \
    }

/**
 * \brief Macro used to register a non-copyable C++ type to CAMP with automatic metaclass creation
 *
 * Using this macro rather than CAMP_TYPE_NONCOPYABLE will make CAMP automatically call
 * the provided registration function the first time the metaclass is requested.
 * This is useful when you don't want to have to manually call an "init" function to
 * create your metaclass.
 *
 * Every type manipulated by CAMP must be registered with CAMP_TYPE, CAMP_AUTO_TYPE or their NONCOPYABLE versions.
 *
 * \sa CAMP_AUTO_TYPE, CAMP_TYPE_NONCOPYABLE
 */
#define CAMP_AUTO_TYPE_NONCOPYABLE(type, registerFunc) \
    namespace camp \
    { \
        namespace detail \
        { \
            template <> struct StaticTypeId<type> \
            { \
                static const char* get(bool checkRegister = true) \
                { \
                    if (checkRegister) \
                        detail::ensureTypeRegistered(#type, registerFunc); \
                    return #type; \
                } \
                enum {defined = true, copyable = false}; \
            }; \
        } \
    }

/**
 * \brief Macro used to activate the CAMP RTTI system into a hierarchy of classes
 *
 * This macro must be inserted in both base and derived classes if you want CAMP
 * to be able to retrieve the dynamic type of polymorphic objects.
 *
 * Example:
 *
 * \code
 * class MyBase
 * {
 *     CAMP_RTTI()
 * };
 *
 * class MyDerived : public MyBase
 * {
 *     CAMP_RTTI()
 * };
 *
 * MyBase* b = new MyDerived;
 * const camp::Class& mc = camp::classByObject(b);
 * // mc == metaclass of MyDerived
 * \endcode
 */
#define CAMP_RTTI() \
    public: virtual const char* campClassId() const {return camp::detail::staticTypeId(this);} \
    private:

} // namespace camp


#endif // CAMP_CAMPTYPE_HPP
