/****************************************************************************
 **
 ** This file is part of the CAMP library.
 **
 ** The MIT License (MIT)
 **
 ** Copyright (C) 2009-2010 TECHNOGERMA Systems France and/or its subsidiary(-ies).
 ** Contact: Technogerma Systems France Information (contact@technogerma.fr)
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

#include "traits.hpp"
#include <camp/detail/functiontraits.hpp>
#include <camp/detail/objecttraits.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/function_types/function_type.hpp>


static void foo() {}

static int bar(float) {return 0;}

struct Callable {
    void operator () () {}
};

struct NonCallable {
    int x;
};

struct Methods
{
    void foo() {}
};

int intArray[10];

using namespace TraitsTest;

//-----------------------------------------------------------------------------
//                         Tests for camp::*traits
//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE(TRAITS)

// Sanity check: C++11 features and syntax.
BOOST_AUTO_TEST_CASE(cpp11)
{
    foo(); bar(0.f); // to stop warning
    
    static_assert(std::is_function<decltype(foo)>::value, "std::is_function failed");
    static_assert(std::is_function<void ()>::value, "std::is_function failed");
    static_assert(!std::is_function<Callable>::value, "std::is_function failed");
    static_assert(!std::is_function<NonCallable>::value, "std::is_function failed");
    
    typedef void (*foo_t)();
    static_assert(std::is_void< std::result_of<decltype(foo)& ()>::type >::value, "std::result_of failed");
    static_assert(std::is_void< std::result_of<decltype(&foo) ()>::type >::value, "std::result_of failed");
    static_assert(std::is_void< std::result_of<foo_t ()>::type >::value, "std::result_of failed");
    
    typedef int (*bar_t)(float);
    static_assert(std::is_same< std::result_of<decltype(bar)& (float)>::type, int >::value, "std::result_of failed");
    static_assert(std::is_same< std::result_of<decltype(&bar) (float)>::type, int >::value, "std::result_of failed");
    static_assert(std::is_same< std::result_of<bar_t (float)>::type, int >::value, "std::result_of failed");
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(functionTraits)
{
    static_assert( ! camp::detail::FunctionTraits<int>::isFunction, "FunctionTraits<>::isFunction failed");
    static_assert( ! camp::detail::FunctionTraits<float>::isFunction, "FunctionTraits<>::isFunction failed");
    static_assert( ! camp::detail::FunctionTraits<int*>::isFunction, "FunctionTraits<>::isFunction failed");
    static_assert( ! camp::detail::FunctionTraits<char*>::isFunction, "FunctionTraits<>::isFunction failed");
    static_assert( ! camp::detail::FunctionTraits<int**>::isFunction, "FunctionTraits<>::isFunction failed");
    static_assert( ! camp::detail::FunctionTraits<std::string>::isFunction, "FunctionTraits<>::isFunction failed");
    
    static_assert(camp::detail::FunctionTraits<void(void)>::isFunction, "FunctionTraits<>::isFunction failed");
    static_assert(camp::detail::FunctionTraits<void(int)>::isFunction, "FunctionTraits<>::isFunction failed");
    static_assert(camp::detail::FunctionTraits<int(void)>::isFunction, "FunctionTraits<>::isFunction failed");
    static_assert(camp::detail::FunctionTraits<int(char*)>::isFunction, "FunctionTraits<>::isFunction failed");

    static_assert(camp::detail::FunctionTraits<decltype(foo)>::isFunction, "FunctionTraits<>::isFunction failed");
    static_assert(camp::detail::FunctionTraits<decltype(bar)>::isFunction, "FunctionTraits<>::isFunction failed");

    static_assert( ! camp::detail::FunctionTraits<NonCallable>::isFunction, "FunctionTraits<>::isFunction failed");
    // Only reports: function and pointer-to-member types & functors
    static_assert( ! camp::detail::FunctionTraits<Callable>::isFunction, "FunctionTraits<>::isFunction failed");

    static_assert(camp::detail::FunctionTraits<void(Methods::*)()>::isFunction, "FunctionTraits<>::isFunction failed");
    void (Methods::*meth_t)() = &Methods::foo;
    static_assert(camp::detail::FunctionTraits<decltype(meth_t)>::isFunction, "FunctionTraits<>::isFunction failed");
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(objectTraits_writable)
{
    // is writable
    static_assert(camp::detail::ObjectTraits<int*>::isWritable, "ObjectTraits<>::isWriteable failed");
    static_assert(camp::detail::ObjectTraits<char**>::isWritable, "ObjectTraits<>::isWriteable failed");

    // is not writable
    static_assert( ! camp::detail::ObjectTraits<int>::isWritable, "ObjectTraits<>::isWriteable failed");
    static_assert( ! camp::detail::ObjectTraits<const int*>::isWritable, "ObjectTraits<>::isWriteable failed");
    static_assert( ! camp::detail::ObjectTraits<const char * const *>::isWritable, "ObjectTraits<>::isWriteable failed");
    static_assert( ! camp::detail::ObjectTraits<void(...)>::isRef, "ObjectTraits<>::isRef failed");
    static_assert( ! camp::detail::ObjectTraits<decltype(intArray)>::isWritable, "ObjectTraits<>::isWriteable failed");
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(objectTraits_ref)
{
    // is ref
    static_assert(camp::detail::ObjectTraits<int*>::isRef, "ObjectTraits<>::isRef failed");
    static_assert(camp::detail::ObjectTraits<char**>::isRef, "ObjectTraits<>::isRef failed");
    static_assert(camp::detail::ObjectTraits<decltype(intArray)>::isRef, "ObjectTraits<>::isRef failed");
    
    // is not ref
    static_assert( ! camp::detail::ObjectTraits<int>::isRef, "ObjectTraits<>::isRef failed");
    static_assert( ! camp::detail::ObjectTraits<float>::isRef, "ObjectTraits<>::isRef failed");
    static_assert( ! camp::detail::ObjectTraits<void(...)>::isRef, "ObjectTraits<>::isRef failed");
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(objectTraits_refReturnType)
{
    static_assert(std::is_same<int&, camp::detail::ObjectTraits<int>::RefReturnType>::value, "ObjectTraits<>::RefReturnType failed");
    static_assert(std::is_same<float&, camp::detail::ObjectTraits<const float>::RefReturnType>::value, "ObjectTraits<>::RefReturnType failed");
    
    // ref return
    static_assert(std::is_same<int*, camp::detail::ObjectTraits<int*>::RefReturnType>::value, "ObjectTraits<>::RefReturnType failed");
    static_assert(std::is_same<const int*, camp::detail::ObjectTraits<const int*>::RefReturnType>::value, "ObjectTraits<>::RefReturnType failed");
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(objectTraits_pointerType)
{
    // pointer type
    static_assert(std::is_same<int*, camp::detail::ObjectTraits<int*>::PointerType>::value, "ObjectTraits<>::PointerType failed");
    static_assert(std::is_same<const int*, camp::detail::ObjectTraits<const int*>::PointerType>::value, "ObjectTraits<>::PointerType failed");
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(objectTraits_dataType)
{
    static_assert(std::is_same<int, camp::detail::ObjectTraits<int>::DataType>::value, "ObjectTraits<>::DataType failed");
    static_assert(std::is_same<int, camp::detail::ObjectTraits<int*>::DataType>::value, "ObjectTraits<>::DataType failed");
    static_assert(std::is_same<int, camp::detail::ObjectTraits<const int>::DataType>::value, "ObjectTraits<>::DataType failed");
    static_assert(std::is_same<int, camp::detail::ObjectTraits<const int*>::DataType>::value, "ObjectTraits<>::DataType failed");
    static_assert(std::is_same<int, camp::detail::ObjectTraits<int **>::DataType>::value, "ObjectTraits<>::DataType failed");
    static_assert(std::is_same<int, camp::detail::ObjectTraits<decltype(intArray)>::DataType>::value, "ObjectTraits<>::DataType failed");
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(boost_function)
{
    typedef void (*fn1_t)(void);
    static_assert(std::is_same<void(), boost::function_types::function_type<fn1_t>::type>::value, "boost::function_types problem");

    typedef int (*fn2_t)(int,const char*,float&);
    static_assert(std::is_same<int(int,const char*,float&), boost::function_types::function_type<fn2_t>::type>::value, "boost::function_types problem");
    
    struct TestClass {
        int foo(float) {return 0;}
    };

    typedef int (TestClass::*fn3_t)(float);
    static_assert(std::is_same<int(TestClass&,float), boost::function_types::function_type<fn3_t>::type>::value, "boost::function_types problem");
}

BOOST_AUTO_TEST_CASE(boost_callable)
{
    static_assert( ! boost::function_types::is_callable_builtin<void>::value, "boost::callable problem");
    static_assert( ! boost::function_types::is_callable_builtin<int>::value, "boost::callable problem");
    static_assert( ! boost::function_types::is_callable_builtin<char*>::value, "boost::callable problem");
    
    struct TestClass {
        int foo(float) {return 0;}
    };
    static_assert(boost::function_types::is_callable_builtin<void()>::value, "boost::callable problem");
    static_assert(boost::function_types::is_callable_builtin<int(TestClass::*)(float)>::value, "boost::callable problem");
}

BOOST_AUTO_TEST_CASE(boost_result_type)
{
    struct TestClass {
        int foo(float) {return 0;}
    };
    static_assert(std::is_same<int, boost::function_types::result_type<int()>::type>::value, "boost::ret result_type");
    static_assert(std::is_same<int, boost::function_types::result_type<int(TestClass::*)(void)>::type>::value, "boost::ret result_type");
    static_assert(std::is_same<float, boost::function_types::result_type<float(TestClass::*)(void)>::type>::value, "boost::ret result_type");
}

BOOST_AUTO_TEST_SUITE_END()
