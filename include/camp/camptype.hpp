/****************************************************************************
**
** Copyright (C) 2009-2010 TECHNOGERMA Systems France and/or its subsidiary(-ies).
** Contact: Technogerma Systems France Information (contact@technogerma.fr)
**
** This file is part of the CAMP library.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2010 TECHNOGERMA Systems France and/or its subsidiary(-ies).
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
 * class NonCopyable : camp::noncopyable
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
