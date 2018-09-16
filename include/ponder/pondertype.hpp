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
/** \cond NoDocumentation */
#ifndef PONDER_PONDERTYPE_HPP
#define PONDER_PONDERTYPE_HPP
/** \endcond NoDocumentation */

#include <ponder/config.hpp>

namespace ponder {

namespace detail
{
    template <typename T> struct StaticTypeId;
    template <typename T> const char* staticTypeId(const T&);
    PONDER_API void ensureTypeRegistered(const char* id, void (*registerFunc)());
}

/**
 * \file ponder/pondertype.hpp
 *
 * \def PONDER_TYPE(TYPE)
 *
 * \brief Macro used to register a C++ type to Ponder
 *
 * Every type manipulated by Ponder must be registered with PONDER_TYPE(), PONDER_AUTO_TYPE()
 * or their NONCOPYABLE versions.
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
 *
 * PONDER_TYPE(MyClass)
 * PONDER_TYPE(MyClass::MyNestedClass)
 * \endcode
 *
 * \note This macro handles types that contain commas, e.g. `Data<float,int,int>`.
 *
 * \sa PONDER_TYPE(), PONDER_AUTO_TYPE()
 */
#define PONDER_TYPE(...) \
    namespace ponder { \
        namespace detail { \
            template <> struct StaticTypeId<__VA_ARGS__> \
            { \
                static const char* get(bool = true) {return #__VA_ARGS__;} \
                static constexpr bool defined = true, copyable = true; \
            }; \
        } \
    }

/**
 * \brief Macro used to register a C++ type to Ponder with automatic metaclass creation
 *
 * Using this macro rather than PONDER_TYPE() will make Ponder automatically call
 * the provided registration function the first time the metaclass is requested.
 * This is useful when you don't want to have to manually call an "init" function to
 * create your metaclass.
 *
 * Every type manipulated by Ponder must be registered with PONDER_TYPE(), PONDER_AUTO_TYPE()
 * or their NONCOPYABLE versions.
 *
 * \note This macro will fail with types that contain commas, e.g. `Data<float,int,int>`. Instead,
 *       use PONDER_TYPE().
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
 * PONDER_AUTO_TYPE(MyClass, &MyClass::registerMetaClass)
 *
 * void MyClass::registerMetaClass()
 * {
 *     ponder::Class::declare<MyClass>("MyClass")
 *         // ... declarations ... ;
 * }
 * \endcode
 *
 * \sa PONDER_TYPE(), \ref eg_page_shapes
 */
#define PONDER_AUTO_TYPE(TYPE, REGISTER_FN) \
    namespace ponder { \
        namespace detail { \
            template <> struct StaticTypeId<TYPE> { \
                static const char* get(bool checkRegister = true) { \
                    if (checkRegister) \
                        detail::ensureTypeRegistered(#TYPE, REGISTER_FN); \
                    return #TYPE; \
                } \
                static constexpr bool defined = true, copyable = true; \
            }; \
        } \
    }
    // TODO - ensureTypeRegistered() called every time referenced!

/**
 * \brief Macro used to register a non-copyable C++ type to Ponder
 *
 * Disabled copy and assignment cannot be detected at compile-time, thus users have to
 * explicitly tell Ponder when a type is not copyable/assignable. Objects of a non-copyable
 * class can be modified through their metaproperties, but they can't be written with a
 * single call to replace to whole object.
 *
 * Every type manipulated by Ponder must be registered with PONDER_TYPE(), PONDER_AUTO_TYPE()
 * or their NONCOPYABLE versions.
 *
 * Example:
 *
 * \code
 * class NonCopyable : util::NonCopyable
 * {
 *     int x;
 * };
 * PONDER_TYPE_NONCOPYABLE(NonCopyable)
 *
 * class MyClass
 * {
 *     NonCopyable* nc;
 * };
 * PONDER_TYPE(MyClass)
 *
 * MyClass c;
 * const ponder::Class& m1 = ponder::classByObject(c);
 * const ponder::Class& m2 = ponder::classByObject(c.nc);
 * const ponder::Property& p1 = m1.property("nc");
 * const ponder::Property& p2 = m2.property("x");
 * p1.set(c, NonCopyable()); // ERROR
 * p2.set(p1.get(c).to<ponder::UserObject>(), 10); // OK
 * \endcode
 *
 * \sa PONDER_TYPE()
 */
#define PONDER_TYPE_NONCOPYABLE(TYPE) \
    namespace ponder { \
        namespace detail { \
            template <> struct StaticTypeId<TYPE> { \
                static const char* get(bool = true) {return #TYPE;} \
                static constexpr bool defined = true, copyable = true; \
            }; \
        } \
    }

/**
 * \brief Macro used to register a non-copyable C++ type to Ponder with automatic
 *        metaclass creation
 *
 * Using this macro rather than PONDER_TYPE_NONCOPYABLE will make Ponder automatically call
 * the provided registration function the first time the metaclass is requested.
 * This is useful when you don't want to have to manually call an "init" function to
 * create your metaclass.
 *
 * Every type manipulated by Ponder must be registered with PONDER_TYPE(), PONDER_AUTO_TYPE()
 * or their NONCOPYABLE versions.
 *
 * \sa PONDER_AUTO_TYPE(), PONDER_TYPE_NONCOPYABLE()
 */
#define PONDER_AUTO_TYPE_NONCOPYABLE(TYPE, REGISTER_FN) \
    namespace ponder { \
        namespace detail { \
            template <> struct StaticTypeId<TYPE> { \
                static const char* get(bool checkRegister = true) { \
                    if (checkRegister) \
                        detail::ensureTypeRegistered(#TYPE, REGISTER_FN); \
                    return #TYPE; \
                } \
                static constexpr bool defined = true, copyable = true; \
            }; \
        } \
    }

/**
 * \brief Macro used to activate the Ponder RTTI system into a hierarchy of classes
 *
 * This macro must be inserted in both base and derived classes if you want Ponder
 * to be able to retrieve the dynamic type of *polymorphic objects*.
 *
 * \note This macro does not need to be inserted into all Ponder classes being declared,
 *       only ones which would like to support features like downcasting via polymorphism.
 *
 * Example:
 *
 * \code
 * class MyBase
 * {
 *     PONDER_POLYMORPHIC()
 * };
 *
 * class MyDerived : public MyBase
 * {
 *     PONDER_POLYMORPHIC()
 * };
 *
 * MyBase* b = new MyDerived;
 * const ponder::Class& mc = ponder::classByObject(b);
 * // mc == metaclass of MyDerived
 * \endcode
 */
#define PONDER_POLYMORPHIC() \
    public: \
        virtual const char* ponderClassId() const {return ponder::detail::staticTypeId(this);} \
    private:

} // namespace ponder

/** \cond NoDocumentation */
#endif // PONDER_PONDERTYPE_HPP
/** \endcond NoDocumentation */
