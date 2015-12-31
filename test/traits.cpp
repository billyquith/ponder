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
#include <ponder/detail/functiontraits.hpp>
#include <ponder/detail/objecttraits.hpp>
#include <ponder/util.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/function_types/function_type.hpp>
#include <boost/function_types/result_type.hpp>
#include <array>

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
    std::string arr[10];
    int arri[7];
    std::array<int, 6> arrv;
    
    int v;
    const int& getV() const {return v;}
};

int intArray[10];

using namespace TraitsTest;

//-----------------------------------------------------------------------------
//                         Tests for ponder::*traits
//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE(TRAITS)

// Sanity check: C++11 features and syntax.
BOOST_AUTO_TEST_CASE(cpp11)
{
    foo(); bar(0.f); // to stop warning
    
    static_assert(std::is_function<decltype(foo)>::value, "std::is_function failed");
    static_assert(std::is_function<void(void)>::value, "std::is_function failed");
    static_assert( ! std::is_function<Callable>::value, "std::is_function failed");
    static_assert( ! std::is_function<NonCallable>::value, "std::is_function failed");
    static_assert( ! std::is_function<void(Methods::*)()>::value, "std::is_function failed");
    
    typedef void (*foo_t)();
    static_assert(std::is_void< std::result_of<decltype(foo)& ()>::type >::value, "std::result_of failed");
    static_assert(std::is_void< std::result_of<decltype(&foo) ()>::type >::value, "std::result_of failed");
    static_assert(std::is_void< std::result_of<foo_t ()>::type >::value, "std::result_of failed");
    static_assert( std::is_pointer<foo_t>::value, "");

    typedef int (*bar_t)(float);
    static_assert(std::is_same< std::result_of<decltype(bar)& (float)>::type, int >::value, "std::result_of failed");
    static_assert(std::is_same< std::result_of<decltype(&bar) (float)>::type, int >::value, "std::result_of failed");
    static_assert(std::is_same< std::result_of<bar_t (float)>::type, int >::value, "std::result_of failed");
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(functionTraits_isFunction)
{
    static_assert( ! ponder::detail::FunctionTraits<int>::isFunction, "FunctionTraits<>::isFunction failed");
    static_assert( ! ponder::detail::FunctionTraits<float>::isFunction, "FunctionTraits<>::isFunction failed");
    static_assert( ! ponder::detail::FunctionTraits<int*>::isFunction, "FunctionTraits<>::isFunction failed");
    static_assert( ! ponder::detail::FunctionTraits<char*>::isFunction, "FunctionTraits<>::isFunction failed");
    static_assert( ! ponder::detail::FunctionTraits<int**>::isFunction, "FunctionTraits<>::isFunction failed");
    static_assert( ! ponder::detail::FunctionTraits<std::string>::isFunction, "FunctionTraits<>::isFunction failed");
    
    static_assert(ponder::detail::FunctionTraits<void(void)>::isFunction, "FunctionTraits<>::isFunction failed");
    static_assert(ponder::detail::FunctionTraits<void(int)>::isFunction, "FunctionTraits<>::isFunction failed");
    static_assert(ponder::detail::FunctionTraits<int(void)>::isFunction, "FunctionTraits<>::isFunction failed");
    static_assert(ponder::detail::FunctionTraits<int(char*)>::isFunction, "FunctionTraits<>::isFunction failed");

    static_assert(ponder::detail::FunctionTraits<decltype(foo)>::isFunction, "FunctionTraits<>::isFunction failed");
    static_assert(ponder::detail::FunctionTraits<decltype(bar)>::isFunction, "FunctionTraits<>::isFunction failed");

    static_assert( ! ponder::detail::FunctionTraits<NonCallable>::isFunction, "FunctionTraits<>::isFunction failed");
    // Only reports: function and pointer-to-member types & functors
    static_assert( ! ponder::detail::FunctionTraits<Callable>::isFunction, "FunctionTraits<>::isFunction failed");

    static_assert(ponder::detail::FunctionTraits<void(Methods::*)()>::isFunction, "FunctionTraits<>::isFunction failed");
    void (Methods::*meth_t)() = &Methods::foo;
    static_assert(ponder::detail::FunctionTraits<decltype(meth_t)>::isFunction, "FunctionTraits<>::isFunction failed");
}

BOOST_AUTO_TEST_CASE(functionTraits_type)
{
    typedef void (*fn1_t)(void);
    static_assert(std::is_same<void(), boost::function_types::function_type<fn1_t>::type>::value, "boost::function_types problem");
    static_assert(std::is_same<void(), ponder::detail::FunctionTraits<fn1_t>::type>::value, "ponder::detail::FunctionTraits problem");
    
    typedef int (*fn2_t)(int,const char*,float&);
    static_assert(std::is_same<int(int,const char*,float&), boost::function_types::function_type<fn2_t>::type>::value, "boost::function_types problem");
    static_assert(std::is_same<int(int,const char*,float&), ponder::detail::FunctionTraits<fn2_t>::type>::value, "ponder::detail::FunctionTraits problem");
    
    struct TestClass {
        int foo(float) {return 0;}
    };

    typedef int (TestClass::*fn3_t)(float);
    static_assert(std::is_same<int(TestClass&,float), boost::function_types::function_type<fn3_t>::type>::value, "boost::function_types problem");
    static_assert(std::is_same<int(TestClass&,float), ponder::detail::MethodDetails<fn3_t>::FunctionType>::value, "ponder::detail::MethodDetails problem");
    static_assert(std::is_same<int(TestClass&,float), ponder::detail::FunctionTraits<fn3_t>::type>::value, "ponder::detail::FunctionTraits problem");
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(objectTraits_writable)
{
    // is writable
    static_assert(ponder::detail::ObjectTraits<int*>::isWritable, "ObjectTraits<>::isWriteable failed");
    static_assert(ponder::detail::ObjectTraits<char**>::isWritable, "ObjectTraits<>::isWriteable failed");

    // is not writable
    static_assert( ! ponder::detail::ObjectTraits<int>::isWritable, "ObjectTraits<>::isWriteable failed");
    static_assert( ! ponder::detail::ObjectTraits<const int*>::isWritable, "ObjectTraits<>::isWriteable failed");
    static_assert( ! ponder::detail::ObjectTraits<const char * const *>::isWritable, "ObjectTraits<>::isWriteable failed");
    static_assert( ! ponder::detail::ObjectTraits<void(...)>::isRef, "ObjectTraits<>::isRef failed");
    static_assert( ! ponder::detail::ObjectTraits<decltype(intArray)>::isWritable, "ObjectTraits<>::isWriteable failed");
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(objectTraits_ref)
{
    // is ref
    static_assert(ponder::detail::ObjectTraits<int*>::isRef, "ObjectTraits<>::isRef failed");
    static_assert(ponder::detail::ObjectTraits<char**>::isRef, "ObjectTraits<>::isRef failed");
    static_assert(ponder::detail::ObjectTraits<decltype(intArray)>::isRef, "ObjectTraits<>::isRef failed");
    
    // is not ref
    static_assert( ! ponder::detail::ObjectTraits<int>::isRef, "ObjectTraits<>::isRef failed");
    static_assert( ! ponder::detail::ObjectTraits<float>::isRef, "ObjectTraits<>::isRef failed");
    static_assert( ! ponder::detail::ObjectTraits<void(...)>::isRef, "ObjectTraits<>::isRef failed");
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(objectTraits_refReturnType)
{
    static_assert(std::is_same<int&, ponder::detail::ObjectTraits<int>::RefReturnType>::value, "ObjectTraits<>::RefReturnType failed");
    static_assert(std::is_same<float&, ponder::detail::ObjectTraits<const float>::RefReturnType>::value, "ObjectTraits<>::RefReturnType failed");
    
    // ref return
    static_assert(std::is_same<int*, ponder::detail::ObjectTraits<int*>::RefReturnType>::value, "ObjectTraits<>::RefReturnType failed");
    static_assert(std::is_same<const int*, ponder::detail::ObjectTraits<const int*>::RefReturnType>::value, "ObjectTraits<>::RefReturnType failed");
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(objectTraits_pointerType)
{
    // pointer type
    static_assert(std::is_same<int*, ponder::detail::ObjectTraits<int*>::PointerType>::value, "ObjectTraits<>::PointerType failed");
    static_assert(std::is_same<const int*, ponder::detail::ObjectTraits<const int*>::PointerType>::value, "ObjectTraits<>::PointerType failed");
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(objectTraits_dataType)
{
    static_assert(std::is_same<int, ponder::detail::ObjectTraits<int>::DataType>::value, "ObjectTraits<>::DataType failed");
    static_assert(std::is_same<int, ponder::detail::ObjectTraits<int*>::DataType>::value, "ObjectTraits<>::DataType failed");
    static_assert(std::is_same<int, ponder::detail::ObjectTraits<const int>::DataType>::value, "ObjectTraits<>::DataType failed");
    static_assert(std::is_same<int, ponder::detail::ObjectTraits<const int*>::DataType>::value, "ObjectTraits<>::DataType failed");
    static_assert(std::is_same<int, ponder::detail::ObjectTraits<int **>::DataType>::value, "ObjectTraits<>::DataType failed");
    static_assert(std::is_same<int, ponder::detail::ObjectTraits<decltype(intArray)>::DataType>::value, "ObjectTraits<>::DataType failed");
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
        int i;
        int arr[5];
    };
    static_assert(boost::function_types::is_callable_builtin<void()>::value, "boost::callable problem");
    static_assert(boost::function_types::is_callable_builtin<int(TestClass::*)(float)>::value, "boost::callable problem");
    
    static_assert(boost::function_types::is_callable_builtin<int(TestClass::*)>::value, "boost::callable problem");
    static_assert(boost::function_types::is_callable_builtin<int(TestClass::*)[5]>::value, "boost::callable problem");
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

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(lexical_cast_to_string)
{
    const unsigned int ui = 234;
    BOOST_CHECK_EQUAL(ponder::util::convert<std::string>(ui), std::to_string(ui));
    
    const int i = -17;
    BOOST_CHECK_EQUAL(ponder::util::convert<std::string>(i), std::to_string(i));
    
    const float f = 108.75f;
    BOOST_CHECK_EQUAL(ponder::util::convert<std::string>(f), "108.75");

    const double d = 108.125;
    BOOST_CHECK_EQUAL(ponder::util::convert<std::string>(d), "108.125");

    const bool bt = true, bf = false;
    BOOST_CHECK_EQUAL(ponder::util::convert<std::string>(bt), "1");
    BOOST_CHECK_EQUAL(ponder::util::convert<std::string>(bf), "0");
}

BOOST_AUTO_TEST_CASE(lexical_cast_to_bool)
{
    const std::string b1("1");
    BOOST_CHECK_EQUAL(ponder::util::convert<bool>(b1), true);

    const std::string b2("0");
    BOOST_CHECK_EQUAL(ponder::util::convert<bool>(b2), false);

    const std::string bt("true");
    BOOST_CHECK_EQUAL(ponder::util::convert<bool>(bt), true);

    const std::string bf("false");
    BOOST_CHECK_EQUAL(ponder::util::convert<bool>(bf), false);
}

BOOST_AUTO_TEST_CASE(lexical_cast_to_char)
{
    BOOST_CHECK_EQUAL(ponder::util::convert<char>(std::string("0")), '0');
    BOOST_CHECK_EQUAL(ponder::util::convert<char>(std::string("g")), 'g');
    BOOST_CHECK_THROW(ponder::util::convert<char>(std::string()), ponder::util::bad_conversion);
    BOOST_CHECK_THROW(ponder::util::convert<char>(std::string("27")), ponder::util::bad_conversion);
    
    BOOST_CHECK_EQUAL(ponder::util::convert<unsigned char>(std::string("0")), '0');
    BOOST_CHECK_EQUAL(ponder::util::convert<unsigned char>(std::string("g")), 'g');
    BOOST_CHECK_THROW(ponder::util::convert<unsigned char>(std::string()), ponder::util::bad_conversion);
    BOOST_CHECK_THROW(ponder::util::convert<unsigned char>(std::string("27")), ponder::util::bad_conversion);
}

BOOST_AUTO_TEST_CASE(lexical_cast_to_short)
{
    BOOST_CHECK_EQUAL(ponder::util::convert<short>(std::string("0")), 0);
    BOOST_CHECK_EQUAL(ponder::util::convert<short>(std::string("2600")), 2600);
    BOOST_CHECK_EQUAL(ponder::util::convert<short>(std::string("-27")), -27);
    
    BOOST_CHECK_EQUAL(ponder::util::convert<unsigned short>(std::string("0")), 0);
    BOOST_CHECK_EQUAL(ponder::util::convert<unsigned short>(std::string("2600")), 2600u);
    BOOST_CHECK_EQUAL(ponder::util::convert<unsigned short>(std::string("-27")), static_cast<unsigned short>(-27u));
}

BOOST_AUTO_TEST_CASE(lexical_cast_to_int)
{
    BOOST_CHECK_EQUAL(ponder::util::convert<int>(std::string("0")), 0);
    BOOST_CHECK_EQUAL(ponder::util::convert<int>(std::string("123456789")), 123456789);
    BOOST_CHECK_EQUAL(ponder::util::convert<int>(std::string("-27")), -27);

    BOOST_CHECK_EQUAL(ponder::util::convert<unsigned int>(std::string("0")), 0);
    BOOST_CHECK_EQUAL(ponder::util::convert<unsigned int>(std::string("123456789")), 123456789u);
    BOOST_CHECK_EQUAL(ponder::util::convert<unsigned int>(std::string("-27")), -27u);
    
    BOOST_CHECK_THROW(ponder::util::convert<int>(std::string("bad number")), ponder::util::bad_conversion);
}

BOOST_AUTO_TEST_CASE(lexical_cast_to_long)
{
    BOOST_CHECK_EQUAL(ponder::util::convert<long long>(std::string("0")), 0);
    BOOST_CHECK_EQUAL(ponder::util::convert<long long>(std::string("1125899906842624")), 1125899906842624ll);
    BOOST_CHECK_EQUAL(ponder::util::convert<long long>(std::string("-27")), -27);
    
    BOOST_CHECK_EQUAL(ponder::util::convert<unsigned long long>(std::string("0")), 0);
    BOOST_CHECK_EQUAL(ponder::util::convert<unsigned long long>(std::string("1125899906842624")), 1125899906842624ull);
    BOOST_CHECK_EQUAL(ponder::util::convert<unsigned long long>(std::string("-27")), -27ull);
}

BOOST_AUTO_TEST_CASE(lexical_cast_to_float)
{
    BOOST_CHECK_EQUAL(ponder::util::convert<float>(std::string("0")), 0);
    BOOST_CHECK_EQUAL(ponder::util::convert<float>(std::string("100.25")), 100.25f);
    BOOST_CHECK_EQUAL(ponder::util::convert<float>(std::string("-27.75")), -27.75f);
}

BOOST_AUTO_TEST_CASE(lexical_cast_to_double)
{
    BOOST_CHECK_EQUAL(ponder::util::convert<double>(std::string("0")), 0);
    BOOST_CHECK_EQUAL(ponder::util::convert<double>(std::string("100.25")), 100.25);
    BOOST_CHECK_EQUAL(ponder::util::convert<double>(std::string("-27.75")), -27.75);
}

//-----------------------------------------------------------------------------

// From: http://en.cppreference.com/w/cpp/utility/integer_sequence

template<typename R, typename Array, std::size_t... I>
R a2t_impl(const Array& a, ponder::util::index_sequence<I...>)
{
    return std::make_tuple(a[I]...);
}

template<typename R, typename T, std::size_t N, typename Indices = ponder::util::make_index_sequence<N>>
R a2t(const std::array<T, N>& a)
{
    return a2t_impl<R>(a, Indices());
}

BOOST_AUTO_TEST_CASE(integer_sequence)
{
    auto is = ponder::util::make_index_sequence<3>();
    BOOST_CHECK_EQUAL(is.size(), 3);

    std::array<int, 4> array {{1,2,3,4}};
    
    // convert an array into a tuple
    auto tuple = a2t<std::tuple<int, int, int, int>>(array);
    static_assert(std::is_same<decltype(tuple), std::tuple<int, int, int, int>>::value, "");
}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(util_allTrue)
{
    BOOST_CHECK_EQUAL(ponder::util::allTrue(), true);
    BOOST_CHECK_EQUAL(ponder::util::allTrue(true), true);
    BOOST_CHECK_EQUAL(ponder::util::allTrue(true, true), true);
    BOOST_CHECK_EQUAL(ponder::util::allTrue(true, true, true), true);
    BOOST_CHECK_EQUAL(ponder::util::allTrue(true, true, true, true), true);
    BOOST_CHECK_EQUAL(ponder::util::allTrue(true, true, true, true, true), true);
    BOOST_CHECK_EQUAL(ponder::util::allTrue(true, true, true, true, true, true), true);
    BOOST_CHECK_EQUAL(ponder::util::allTrue(true, true, true, true, true, true, true), true);
    BOOST_CHECK_EQUAL(ponder::util::allTrue(true, true, true, true, true, true, true, true), true);
    
    BOOST_CHECK_EQUAL(ponder::util::allTrue(false), false);
    BOOST_CHECK_EQUAL(ponder::util::allTrue(true, false), false);
    BOOST_CHECK_EQUAL(ponder::util::allTrue(true, false, true), false);
    BOOST_CHECK_EQUAL(ponder::util::allTrue(true, true, false, true), false);
    BOOST_CHECK_EQUAL(ponder::util::allTrue(true, true, true, false, true), false);
    BOOST_CHECK_EQUAL(ponder::util::allTrue(true, false, true, true, true, true), false);
    BOOST_CHECK_EQUAL(ponder::util::allTrue(true, true, false, true, true, true, true), false);
    BOOST_CHECK_EQUAL(ponder::util::allTrue(true, false, true, true, true, false, true, true), false);
}

BOOST_AUTO_TEST_SUITE_END()



