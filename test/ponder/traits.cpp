/****************************************************************************
 **
 ** This file is part of the Ponder library, formerly CAMP.
 **
 ** The MIT License (MIT)
 **
 ** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
 ** Copyright (C) 2015-2020 Nick Trout.
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

// Test std and ponder traits, on which this whole API is based.
//  - Makes sure compiler and std implementation are as expected.
//  - Test Ponder traits work as expected.

#include <ponder/classbuilder.hpp>
#include "test.hpp"
#include <array>
#include <string.h> // strcmp()

#ifdef TEST_BOOST
#include <boost/function_types/function_type.hpp>
#include <boost/function_types/result_type.hpp>
#endif

namespace TraitsTest
{
    static void func() {}

    static int funcArgReturn(float) {return 0;}

    struct Class
    {
        float method(float x) const { return x*3; }
        static int staticFunc() { return 77; }
    };

    struct Methods
    {
        void foo() {}
        ponder::String arr[10];
        int arri[7];
        std::array<int, 6> arrv;

        int v;
        const int& getV() const {return v;}
    };

    struct Callable {
        void operator () (void) {}
    };

    struct NonCallable {
        int x;
    };

    int intArray[10];

    template <class T>
    class TemplateClass
    {
    public:
        T x;
    };

    enum Enum { RED, GREEN, BLUE };
    enum class EnumCls { TALL, SHORT };

    struct FuncReturn
    {
        int m_i;
        Callable m_c;

        int i() const {return 0;}
        float f() {return 2.4f;}

        int* ip() {return &m_i;}

        std::shared_ptr<Callable> sp() {return std::make_shared<Callable>(m_c);}

        std::vector<int> ai() {return std::vector<int>{1,2,3};}

        std::vector<std::shared_ptr<Callable>> as()
        {
            return std::vector<std::shared_ptr<Callable>>();
        }
    };

    class Dummy {};
    
    void declare()
    {
        ponder::Class::declare<Class>();
    }
}

using namespace TraitsTest;

PONDER_TYPE(TraitsTest::Class);

//-----------------------------------------------------------------------------
//                         Tests for ponder traits
//-----------------------------------------------------------------------------

// Sanity check: make sure compiler supports features we need.
TEST_CASE("C++11 features and syntax")
{
    SECTION("function test")
    {
        func(); funcArgReturn(0.f); // to stop unused warning

        static_assert(std::is_function<decltype(func)>::value, "std::is_function failed");
        static_assert(std::is_function<void(void)>::value, "std::is_function failed");
        static_assert(std::is_function<
                        std::remove_pointer<decltype(&Class::staticFunc)>::type>::value,
                      "std::is_function failed");
        static_assert(std::is_function< std::function<void()>() >::value,
                      "std::is_function failed");

        static_assert( ! std::is_function<Callable>::value, "std::is_function failed");
        static_assert( ! std::is_function<NonCallable>::value, "std::is_function failed");
        static_assert( ! std::is_function<decltype(&Class::method)>::value,
                      "std::is_function failed");
        static_assert( ! std::is_function<void(Methods::*)()>::value, "std::is_function failed");
        static_assert( ! std::is_function<
                            std::remove_pointer<decltype(&Methods::foo)>::type>::value,
                      "std::is_function failed");
    }

#if defined(__cpp_lib_constexpr_invoke)
    SECTION("function result")
    {
        STATIC_ASSERT((std::is_void<typename std::invoke_result_t<void()>>::value));

        STATIC_ASSERT(std::is_void< typename std::invoke_result_t<decltype(func)> >::value);
        STATIC_ASSERT(std::is_void< typename std::invoke_result_t<decltype(&func)> >::value);

        typedef void (foo_t)();
        STATIC_ASSERT(std::is_void< typename std::invoke_result_t<foo_t> >::value);
        STATIC_ASSERT(!std::is_pointer<foo_t>::value);
    }
#endif // __cpp_lib_constexpr_invoke

    SECTION("arrays")
    {
        static_assert( ! std::is_array<int>::value, "std::is_array failed");
        static_assert(   std::is_array<int[10]>::value, "std::is_array failed");
        static_assert( ! std::is_array<int*>::value, "std::is_array failed");
        static_assert( ! std::is_array<int*>::value, "std::is_array failed");
    }

    SECTION("other")
    {
        static_assert(std::is_const<const int>::value, "std::is_array failed");
        static_assert(std::is_const<std::remove_reference<const int&>::type>::value, "std::is_array failed");
        
        static_assert(std::is_enum<Enum>::value, "");
    }
}

//----------------------------------------------------------------------------------------

TEST_CASE("Ponder supports different function types")
{
    SECTION("what is not a function type")
    {
        using ponder::detail::FunctionTraits;
        using ponder::FunctionKind;

        STATIC_ASSERT(FunctionTraits<int>::kind == FunctionKind::None);
        STATIC_ASSERT(FunctionTraits<float>::kind == FunctionKind::None);
        STATIC_ASSERT(FunctionTraits<int*>::kind == FunctionKind::None);
        STATIC_ASSERT(FunctionTraits<char*>::kind == FunctionKind::None);
        STATIC_ASSERT(FunctionTraits<int**>::kind == FunctionKind::None);
        STATIC_ASSERT(FunctionTraits<ponder::String>::kind == FunctionKind::None);
        STATIC_ASSERT(FunctionTraits<NonCallable>::kind == FunctionKind::None);
    }

    SECTION("type function")    // T(*)()
    {
        using ponder::detail::FunctionTraits;
        using ponder::PropertyKind;
        using ponder::FunctionKind;

        STATIC_ASSERT((ponder::detail::PropertyFactory1<Dummy, void(void)>::kind == PropertyKind::Function));
        STATIC_ASSERT(FunctionTraits<void(void)>::kind == FunctionKind::Function);

        STATIC_ASSERT((ponder::detail::PropertyFactory1<Dummy, void(int)>::kind == PropertyKind::Function));
        STATIC_ASSERT(FunctionTraits<void(int)>::kind == FunctionKind::Function);

        STATIC_ASSERT((ponder::detail::PropertyFactory1<Dummy, int(void)>::kind == PropertyKind::Function));
        STATIC_ASSERT(FunctionTraits<int(void)>::kind == FunctionKind::Function);

        STATIC_ASSERT((ponder::detail::PropertyFactory1<Dummy, int(char*)>::kind == PropertyKind::Function));
        STATIC_ASSERT(FunctionTraits<int(char*)>::kind == FunctionKind::Function);

        // non-class void(void)
        STATIC_ASSERT((ponder::detail::PropertyFactory1<Dummy, decltype(func)>::kind == PropertyKind::Function));
        STATIC_ASSERT(FunctionTraits<decltype(func)>::kind == FunctionKind::Function);

        // non-class R(...)
        STATIC_ASSERT((ponder::detail::PropertyFactory1<Dummy, decltype(funcArgReturn)>::kind == PropertyKind::Function));
        STATIC_ASSERT(FunctionTraits<decltype(funcArgReturn)>::kind == FunctionKind::Function);

        // class static R(void)
        STATIC_ASSERT((ponder::detail::PropertyFactory1<Dummy, decltype(&Class::staticFunc)>::kind == PropertyKind::Function));
        STATIC_ASSERT(FunctionTraits<decltype(&Class::staticFunc)>::kind == FunctionKind::Function);
    }


    SECTION("type member function")  // T(C::*)()
    {
        using ponder::detail::FunctionTraits;
        using ponder::PropertyKind;
        using ponder::FunctionKind;

        STATIC_ASSERT(std::is_member_function_pointer<void (TraitsTest::Methods::*)()>::value);
        STATIC_ASSERT((ponder::detail::PropertyFactory1<Methods, void(Methods::*)()>::kind == PropertyKind::Function));
        STATIC_ASSERT(FunctionTraits<void(Methods::*)()>::kind == FunctionKind::MemberFunction);

        void (Methods::*meth_t)() = &Methods::foo;
        STATIC_ASSERT((ponder::detail::PropertyFactory1<Methods, decltype(meth_t)>::kind == PropertyKind::Function));
        STATIC_ASSERT(FunctionTraits<decltype(meth_t)>::kind == FunctionKind::MemberFunction);
    }

    SECTION("type member object")   // T(C::*)
    {
        using ponder::detail::FunctionTraits;
        using ponder::PropertyKind;
        using ponder::FunctionKind;

        struct Members {
            int m;
            float a[5];
            bool b[8];
        };

        STATIC_ASSERT((ponder::detail::PropertyFactory1<Dummy, int Members::*>::kind == PropertyKind::MemberObject));

        STATIC_ASSERT((ponder::detail::PropertyFactory1<Dummy, float (Members::*)[]>::kind == PropertyKind::MemberObject));

        STATIC_ASSERT((ponder::detail::PropertyFactory1<Dummy, decltype(&Members::a)>::kind == PropertyKind::MemberObject));

        STATIC_ASSERT((ponder::detail::PropertyFactory1<Dummy, decltype(&Members::b)>::kind == PropertyKind::MemberObject));
    }

    SECTION("type function wrapper")  // std::function<>
    {
        using ponder::detail::FunctionTraits;
        using ponder::PropertyKind;
        using ponder::FunctionKind;

        STATIC_ASSERT((ponder::detail::PropertyFactory1<Dummy, std::function<void()>>::kind == PropertyKind::Function));
        STATIC_ASSERT(FunctionTraits<std::function<void()>>::kind == FunctionKind::FunctionWrapper);

        STATIC_ASSERT((ponder::detail::PropertyFactory1<Dummy, std::function<int(float,int)>>::kind == PropertyKind::Function));
        STATIC_ASSERT(FunctionTraits<std::function<int(float,int)>>::kind == FunctionKind::FunctionWrapper);

        STATIC_ASSERT((ponder::detail::PropertyFactory1<Dummy, std::function<char*(char[])>>::kind == PropertyKind::Function));
        STATIC_ASSERT(FunctionTraits<std::function<char*(char[])>>::kind == FunctionKind::FunctionWrapper);
    }

    SECTION("type lambda")  // [] () {}
    {
        using ponder::detail::FunctionTraits;
        using ponder::PropertyKind;
        using ponder::FunctionKind;

        auto l1 = [] () {};
        auto l2 = [=] (int&) { return "hello"; };
        auto l3 = [] (float a, float b) -> float { return a*b; };
        
        std::function<void()> f1(l1);

        STATIC_ASSERT((ponder::detail::PropertyFactory1<Dummy, decltype(l1)>::kind == PropertyKind::Function));
        STATIC_ASSERT(FunctionTraits<decltype(l1)>::kind == FunctionKind::Lambda);

        STATIC_ASSERT((ponder::detail::PropertyFactory1<Dummy, decltype(l2)>::kind == PropertyKind::Function));
        STATIC_ASSERT(FunctionTraits<decltype(l2)>::kind == FunctionKind::Lambda);

        typedef decltype(l3) L3Type;
        STATIC_ASSERT((ponder::detail::PropertyFactory1<Dummy, L3Type>::kind == PropertyKind::Function));
        typedef FunctionTraits<L3Type> L3Traits;
        STATIC_ASSERT(L3Traits::kind == FunctionKind::Lambda);
    }
}

TEST_CASE("Functions have access types")
{
    SECTION("functions can return values")
    {
        using ponder::detail::FunctionTraits;
        using ponder::PropertyKind;
        using ponder::FunctionKind;

        typedef decltype(&FuncReturn::i) fn;
        STATIC_ASSERT((ponder::detail::PropertyFactory1<Dummy, fn>::kind == PropertyKind::Function));
        STATIC_ASSERT(FunctionTraits<fn>::kind == FunctionKind::MemberFunction);

        STATIC_ASSERT((std::is_same<
                         FunctionTraits<decltype(&FuncReturn::i)>::ExposedType, int
                       >::value));
        STATIC_ASSERT((std::is_same<
                         FunctionTraits<decltype(&FuncReturn::f)>::ExposedType, float
                       >::value));
        STATIC_ASSERT((std::is_same<
                         FunctionTraits<decltype(&FuncReturn::ip)>::ExposedType, int*
                       >::value));

        STATIC_ASSERT((std::is_same<
                         FunctionTraits<decltype(&FuncReturn::sp)>::ExposedType, \
                         std::shared_ptr<Callable> \
                       >::value));

//        STATIC_ASSERT(std::is_same<
//                        FunctionTraits<decltype(&FuncReturn::ai)>::ExposedType,
//                        std::vector<int>
//                      >::value, "");
//        STATIC_ASSERT(std::is_same<
//                        FunctionTraits<decltype(&FuncReturn::as)>::ExposedType,
//                        std::vector<std::shared_ptr<Callable>>
//                      >::value, "");
    }
}

//----------------------------------------------------------------------------------------

// We reference objects in different ways
TEST_CASE("ReferenceTrait kinds")
{
    SECTION("object instance")
    {
        using ponder::detail::TypeTraits;
        using ponder::ReferenceKind;

        STATIC_ASSERT(TypeTraits<int>::kind == ReferenceKind::Instance);
        STATIC_ASSERT(TypeTraits<float>::kind == ReferenceKind::Instance);
        STATIC_ASSERT(TypeTraits<Methods>::kind == ReferenceKind::Instance);
        STATIC_ASSERT(TypeTraits<const int>::kind == ReferenceKind::Instance);
        STATIC_ASSERT(TypeTraits<const float>::kind == ReferenceKind::Instance);
        STATIC_ASSERT(TypeTraits<const Methods>::kind == ReferenceKind::Instance);

        STATIC_ASSERT(TypeTraits<int*>::kind != ReferenceKind::Instance);
    }

    SECTION("type pointer")
    {
        using ponder::detail::TypeTraits;
        using ponder::ReferenceKind;

        STATIC_ASSERT(TypeTraits<int*>::kind == ReferenceKind::Pointer);
        STATIC_ASSERT(TypeTraits<float*>::kind == ReferenceKind::Pointer);
        STATIC_ASSERT(TypeTraits<Methods*>::kind == ReferenceKind::Pointer);

        STATIC_ASSERT(TypeTraits<int**>::kind == ReferenceKind::Pointer);
        STATIC_ASSERT(TypeTraits<float**>::kind == ReferenceKind::Pointer);
        STATIC_ASSERT(TypeTraits<Methods**>::kind == ReferenceKind::Pointer);
    }

    SECTION("type reference")
    {
        using ponder::detail::TypeTraits;
        using ponder::ReferenceKind;

        STATIC_ASSERT(TypeTraits<int&>::kind == ReferenceKind::Reference);
        STATIC_ASSERT(TypeTraits<float&>::kind == ReferenceKind::Reference);
        STATIC_ASSERT(TypeTraits<Methods&>::kind == ReferenceKind::Reference);

        STATIC_ASSERT(TypeTraits<const int&>::kind == ReferenceKind::Reference);
        STATIC_ASSERT(TypeTraits<const float&>::kind == ReferenceKind::Reference);
        STATIC_ASSERT(TypeTraits<const Methods&>::kind == ReferenceKind::Reference);
    }

    SECTION("type smart pointer")
    {
        using ponder::detail::TypeTraits;
        using ponder::ReferenceKind;

        //STATIC_ASSERT(TypeTraits<std::unique_ptr<Methods>>::kind == ReferenceKind::SmartPointer);
        STATIC_ASSERT(TypeTraits<std::shared_ptr<Methods>>::kind == ReferenceKind::SmartPointer);
    }

//    SECTION("type builtin array")
//    {
//        using ponder::detail::TypeTraits;
//        using ponder::ReferenceKind;
//
//        static_assert(TypeTraits<int[1]>::kind == ReferenceKind::BuiltinArray,
//                      "TypeTraits<>::kind");
//        static_assert(TypeTraits<int[1000]>::kind == ReferenceKind::BuiltinArray,
//                      "TypeTraits<>::kind");
//        static_assert(TypeTraits<float[1]>::kind == ReferenceKind::BuiltinArray,
//                      "TypeTraits<>::kind");
//        static_assert(TypeTraits<Methods[10]>::kind == ReferenceKind::BuiltinArray,
//                      "TypeTraits<>::kind");
//        static_assert(TypeTraits<int[10][10]>::kind == ReferenceKind::BuiltinArray,
//                      "TypeTraits<>::kind");
//        static_assert(TypeTraits<int[10][20][30]>::kind == ReferenceKind::BuiltinArray,
//                      "TypeTraits<>::kind");
//    }
}

//----------------------------------------------------------------------------------------

TEST_CASE("Referenced objects have traits")
{
    SECTION("types can be tested for being writable")
    {
        using ponder::detail::TypeTraits;
        using ponder::ReferenceKind;
        
        // is writable
        STATIC_ASSERT(TypeTraits<int>::isWritable);
        STATIC_ASSERT(TypeTraits<int*>::isWritable);
        STATIC_ASSERT(TypeTraits<char**>::isWritable);
        
        // is not writable
        STATIC_ASSERT( ! TypeTraits<const int>::isWritable);
        STATIC_ASSERT( ! TypeTraits<const int*>::isWritable);
        //        static_assert( ! TypeTraits<const char * const *>::isWritable,
        //                      "TypeTraits<>::isWriteable failed");
        //        static_assert( ! TypeTraits<void(...)>::isRef,
        //                      "TypeTraits<>::isRef failed");
        //        static_assert( ! TypeTraits<decltype(intArray)>::isWritable,
        //                      "TypeTraits<>::isWriteable failed");
    }
    
    SECTION("types can be references")
    {
        using ponder::detail::TypeTraits;
        using ponder::ReferenceKind;
        
        // is ref
        STATIC_ASSERT(TypeTraits<int*>::isRef);
        STATIC_ASSERT(TypeTraits<char**>::isRef);
        
        STATIC_ASSERT(ponder::detail::TypeTraits<int&>::isRef);
        STATIC_ASSERT(ponder::detail::TypeTraits<int&>::kind == ReferenceKind::Reference);
        //        static_assert(ponder::detail::TypeTraits<int*&>::isRef,
        //                      "TypeTraits<>::isRef failed");
        
        // is not ref
        STATIC_ASSERT( ! TypeTraits<int>::isRef);
        STATIC_ASSERT( ! TypeTraits<float>::isRef);
        STATIC_ASSERT( ! TypeTraits<void>::isRef);
        //        static_assert( ! TypeTraits<decltype(intArray)>::isRef,
        //                      "TypeTraits<>::isRef failed");
        STATIC_ASSERT( ! ponder::detail::TypeTraits<Callable>::isRef);
        STATIC_ASSERT(ponder::detail::TypeTraits<Callable>::kind != ReferenceKind::Reference);
        STATIC_ASSERT( ! ponder::detail::TypeTraits<NonCallable>::isRef);
    }
    
    SECTION("types can be const")
    {
        using ponder::detail::TypeTraits;
        using ponder::ReferenceKind;
        
        STATIC_ASSERT(!TypeTraits<const int*>::isWritable);
        STATIC_ASSERT(TypeTraits<int*>::isWritable);
        
        STATIC_ASSERT(!TypeTraits<const int&>::isWritable);
        STATIC_ASSERT(TypeTraits<int&>::isWritable);
        
        //        STATIC_ASSERT(!TypeTraits<const int[3]>::isWritable, "TypeTraits<>::isWritable failed");
        //        STATIC_ASSERT(TypeTraits<int[5]>::isWritable, "TypeTraits<>::isWritable failed");
        
        //STATIC_ASSERT(!TypeTraits<const std::vector<float>::isWritable, "TypeTraits<>::isWritable failed");
        //STATIC_ASSERT(TypeTraits<std::vector<float>>::isWritable, "TypeTraits<>::isWritable failed");
    }
    
    SECTION("types can be converted to reference types")
    {
        using ponder::detail::TypeTraits;
        
        STATIC_ASSERT((std::is_same<int&, TypeTraits<int>::ReferenceType>::value));
        STATIC_ASSERT((std::is_same<const float&, TypeTraits<const float>::ReferenceType>::value));
        
        // ref return
        STATIC_ASSERT((std::is_same<int*, TypeTraits<int*>::ReferenceType>::value));
        STATIC_ASSERT((std::is_same<const int*, TypeTraits<const int*>::ReferenceType>::value));
    }
    
    SECTION("types can be pointers")
    {
        using ponder::detail::TypeTraits;
        
        // pointer type
        STATIC_ASSERT((std::is_same<int*, TypeTraits<int*>::PointerType>::value));
        STATIC_ASSERT((std::is_same<const int*, TypeTraits<const int*>::PointerType>::value));
    }
    
    SECTION("is a smart pointer")
    {
        using ponder::detail::IsSmartPointer;
        
        STATIC_ASSERT((IsSmartPointer<int, int>::value == false));
        
        STATIC_ASSERT((IsSmartPointer<int*, int>::value == false));
        
        STATIC_ASSERT((IsSmartPointer<const int*, int>::value == false));
        
        STATIC_ASSERT((IsSmartPointer<TraitsTest::TemplateClass<int>, int>::value == false));
        
        STATIC_ASSERT((IsSmartPointer<std::unique_ptr<int>, int>::value == true));
        
        STATIC_ASSERT((IsSmartPointer<std::shared_ptr<int>, int>::value == true));
    }
    
    SECTION("types have a raw data type")
    {
        using ponder::detail::TypeTraits;
        
        STATIC_ASSERT((std::is_same<int, TypeTraits<int>::DataType>::value));
        STATIC_ASSERT((std::is_same<int, TypeTraits<int*>::DataType>::value));
        STATIC_ASSERT((std::is_same<int, TypeTraits<const int>::DataType>::value));
        STATIC_ASSERT((std::is_same<int, TypeTraits<const int*>::DataType>::value));
        STATIC_ASSERT((std::is_same<int, TypeTraits<int **>::DataType>::value));
        //        STATIC_ASSERT(
        //            std::is_same<int, TypeTraits<int[10]>::DataType>::value,
        //            "TypeTraits<>::DataType failed");
    }
}

//----------------------------------------------------------------------------------------

TEST_CASE("AccessTraits")
{
    SECTION("Simple")
    {
        using ponder::detail::AccessTraits;
        using ponder::PropertyAccessKind;
        
        STATIC_ASSERT(AccessTraits<bool>::kind == PropertyAccessKind::Simple);
        STATIC_ASSERT(AccessTraits<int>::kind == PropertyAccessKind::Simple);
        STATIC_ASSERT(AccessTraits<float>::kind == PropertyAccessKind::Simple);
        STATIC_ASSERT(AccessTraits<std::string>::kind == PropertyAccessKind::Simple);
    }
    
    SECTION("Enum")
    {
        using ponder::detail::AccessTraits;
        using ponder::PropertyAccessKind;
        
        STATIC_ASSERT(AccessTraits<Enum>::kind == PropertyAccessKind::Enum);
        STATIC_ASSERT(AccessTraits<EnumCls>::kind == PropertyAccessKind::Enum);
    }
    
    SECTION("Array")
    {
        using ponder::detail::AccessTraits;
        using ponder::PropertyAccessKind;
        
        STATIC_ASSERT(AccessTraits<int[10]>::kind == PropertyAccessKind::Container);
    }
    
    SECTION("User")
    {
        using ponder::detail::AccessTraits;
        using ponder::PropertyAccessKind;
        
        STATIC_ASSERT(ponder::detail::hasStaticTypeDecl<Class>());
        
        static_assert(AccessTraits<Class>::kind == PropertyAccessKind::User);
        //static_assert(AccessTraits<Class&>::kind == PropertyAccessKind::User, "");
    }
}

//----------------------------------------------------------------------------------------

TEST_CASE("Type testing")
{
    SECTION("find raw type of any type")
    {
        using ponder::detail::DataType;

        STATIC_ASSERT((std::is_same<int, DataType<int>::Type>::value));
        STATIC_ASSERT((std::is_same<int, DataType<int*>::Type>::value));
        STATIC_ASSERT((std::is_same<int, DataType<int**>::Type>::value));
        STATIC_ASSERT((std::is_same<int, DataType<int***>::Type>::value));
        STATIC_ASSERT((std::is_same<int, DataType<int&>::Type>::value));

        STATIC_ASSERT((std::is_same<char, DataType<char>::Type>::value));
        STATIC_ASSERT((std::is_same<float, DataType<float*>::Type>::value));

        STATIC_ASSERT((std::is_same<std::string, DataType<std::string>::Type>::value));
        STATIC_ASSERT((std::is_same<std::string, DataType<std::string&>::Type>::value));
        STATIC_ASSERT((std::is_same<std::string, DataType<const std::string&>::Type>::value));
        STATIC_ASSERT((std::is_same<std::string, DataType<std::string*>::Type>::value));
        STATIC_ASSERT((std::is_same<std::string, DataType<const std::string*>::Type>::value));

        STATIC_ASSERT((std::is_same<Callable, DataType<Callable>::Type>::value));
        STATIC_ASSERT((std::is_same<Callable, DataType<Callable*>::Type>::value));
        STATIC_ASSERT((std::is_same<Callable, DataType<Callable&>::Type>::value));
    }

    SECTION("which are user types")
    {
        using ponder::detail::IsUserType;

        STATIC_ASSERT(( ! IsUserType<int>::value));
        STATIC_ASSERT(( ! IsUserType<char*>::value));
        STATIC_ASSERT(( ! IsUserType<std::string>::value));
        STATIC_ASSERT(( ! IsUserType<ponder::Value>::value));
        STATIC_ASSERT(( ! IsUserType<Enum>::value));
        STATIC_ASSERT(( ! IsUserType<EnumCls>::value));

        STATIC_ASSERT((IsUserType<Callable>::value));
        STATIC_ASSERT((IsUserType<NonCallable>::value));
    }

    SECTION("which are user types")
    {
        using ponder::detail::IsUserObjRef;

        STATIC_ASSERT(( ! IsUserObjRef<int>::value));
        STATIC_ASSERT(( ! IsUserObjRef<Callable>::value));
        STATIC_ASSERT(( ! IsUserObjRef<NonCallable>::value));

        STATIC_ASSERT((IsUserObjRef<NonCallable*>::value));
        STATIC_ASSERT((IsUserObjRef<NonCallable&>::value));
        STATIC_ASSERT((IsUserObjRef<const NonCallable&>::value));
    }
}

//----------------------------------------------------------------------------------------

TEST_CASE("Types supporting array interface are supported")
{
    SECTION("not arrays")
    {
        STATIC_ASSERT(( ! ponder_ext::ArrayMapper<int>::isArray));
        STATIC_ASSERT(( ! ponder_ext::ArrayMapper<char*>::isArray));
    }

    SECTION("C arrays")
    {
        STATIC_ASSERT((ponder_ext::ArrayMapper<int[10]>::isArray));
        STATIC_ASSERT((std::is_same<int, ponder_ext::ArrayMapper<int[10]>::ElementType>::value));
    }

    SECTION("std::array")
    {
        STATIC_ASSERT((ponder_ext::ArrayMapper<std::array<int, 10>>::isArray));
        STATIC_ASSERT((std::is_same<int, ponder_ext::ArrayMapper<std::array<int, 10>>::ElementType>::value));
    }

    SECTION("std::vector")
    {
        STATIC_ASSERT((ponder_ext::ArrayMapper<std::vector<int>>::isArray));
        static_assert(std::is_same<int, ponder_ext::ArrayMapper<std::vector<int>>::ElementType>::value);
    }

    SECTION("std::list")
    {
        STATIC_ASSERT(ponder_ext::ArrayMapper<std::list<int>>::isArray);
        STATIC_ASSERT((std::is_same<int, ponder_ext::ArrayMapper<std::list<int>>::ElementType>::value));
    }
}

//----------------------------------------------------------------------------------------

TEST_CASE("Lexical cast is used")
{
    SECTION("lexical_cast_to_string")
    {
        const unsigned int ui = 234;
        REQUIRE(ponder::detail::convert<ponder::String>(ui) == std::to_string(ui));

        const int i = -17;
        REQUIRE(ponder::detail::convert<ponder::String>(i) == std::to_string(i));

        const float f = 108.75f;
        REQUIRE(ponder::detail::convert<ponder::String>(f) == "108.750000");

        const double d = 108.125;
        REQUIRE(ponder::detail::convert<ponder::String>(d) == "108.125000");

        const bool bt = true, bf = false;
        REQUIRE(ponder::detail::convert<ponder::String>(bt) == "1");
        REQUIRE(ponder::detail::convert<ponder::String>(bf) == "0");
    }

    SECTION("lexical_cast_to_bool")
    {
        const ponder::String b1("1");
        REQUIRE(ponder::detail::convert<bool>(b1) == true);

        const ponder::String b2("0");
        REQUIRE(ponder::detail::convert<bool>(b2) == false);

        const ponder::String bt("true");
        REQUIRE(ponder::detail::convert<bool>(bt) == true);

        const ponder::String bf("false");
        REQUIRE(ponder::detail::convert<bool>(bf) == false);
    }

    SECTION("lexical_cast_to_char")
    {
        REQUIRE(ponder::detail::convert<char>(ponder::String("0")) == '0');
        REQUIRE(ponder::detail::convert<char>(ponder::String("g")) == 'g');
        REQUIRE_THROWS_AS(ponder::detail::convert<char>(ponder::String()),
                          ponder::detail::bad_conversion);
        REQUIRE_THROWS_AS(ponder::detail::convert<char>(ponder::String("27")),
                          ponder::detail::bad_conversion);

        REQUIRE(ponder::detail::convert<unsigned char>(ponder::String("0")) == '0');
        REQUIRE(ponder::detail::convert<unsigned char>(ponder::String("g")) == 'g');
        REQUIRE_THROWS_AS(ponder::detail::convert<unsigned char>(ponder::String()),
                          ponder::detail::bad_conversion);
        REQUIRE_THROWS_AS(ponder::detail::convert<unsigned char>(ponder::String("27")),
                          ponder::detail::bad_conversion);
    }

    SECTION("lexical_cast_to_short")
    {
        REQUIRE(ponder::detail::convert<short>(ponder::String("0")) == 0);
        REQUIRE(ponder::detail::convert<short>(ponder::String("2600")) == 2600);
        REQUIRE(ponder::detail::convert<short>(ponder::String("-27")) == -27);

        REQUIRE(ponder::detail::convert<unsigned short>(ponder::String("0")) == 0);
        REQUIRE(ponder::detail::convert<unsigned short>(ponder::String("2600")) == 2600u);
        REQUIRE(ponder::detail::convert<unsigned short>(ponder::String("-27"))
                == static_cast<unsigned short>(-27));
    }

    SECTION("lexical_cast_to_int")
    {
        REQUIRE(ponder::detail::convert<int>(ponder::String("0")) == 0);
        REQUIRE(ponder::detail::convert<int>(ponder::String("123456789")) == 123456789);
        REQUIRE(ponder::detail::convert<int>(ponder::String("-27")) == -27);

        REQUIRE(ponder::detail::convert<unsigned int>(ponder::String("0")) == 0);
        REQUIRE(ponder::detail::convert<unsigned int>(ponder::String("123456789")) == 123456789u);
        REQUIRE(ponder::detail::convert<unsigned int>(ponder::String("-27"))
                == static_cast<unsigned int>(-27));

        REQUIRE_THROWS_AS(ponder::detail::convert<int>(ponder::String("bad number")),
                          ponder::detail::bad_conversion);
    }

    SECTION("lexical_cast_to_long")
    {
        REQUIRE(ponder::detail::convert<long long>(ponder::String("0")) == 0);
        REQUIRE(ponder::detail::convert<long long>(ponder::String("1125899906842624"))
                == 1125899906842624ll);
        REQUIRE(ponder::detail::convert<long long>(ponder::String("-27")) == -27);

        REQUIRE(ponder::detail::convert<unsigned long long>(ponder::String("0")) == 0);
        REQUIRE(ponder::detail::convert<unsigned long long>(ponder::String("1125899906842624"))
                == 1125899906842624ull);
        REQUIRE(ponder::detail::convert<unsigned long long>(ponder::String("-27"))
                == static_cast<unsigned long long>(-27));
    }

    SECTION("lexical_cast_to_float")
    {
        REQUIRE(ponder::detail::convert<float>(ponder::String("0")) == 0);
        REQUIRE(ponder::detail::convert<float>(ponder::String("100.25")) == 100.25f);
        REQUIRE(ponder::detail::convert<float>(ponder::String("-27.75")) == -27.75f);
    }

    SECTION("lexical_cast_to_double")
    {
        REQUIRE(ponder::detail::convert<double>(ponder::String("0")) == 0);
        REQUIRE(ponder::detail::convert<double>(ponder::String("100.25")) == 100.25);
        REQUIRE(ponder::detail::convert<double>(ponder::String("-27.75")) == -27.75);
    }
}

//----------------------------------------------------------------------------------------

// From: http://en.cppreference.com/w/cpp/utility/integer_sequence

template<typename R, typename Array, size_t... I>
R a2t_impl(const Array& a, PONDER__SEQNS::index_sequence<I...>)
{
    return std::make_tuple(a[I]...);
}

template< typename R,
          typename T,
          size_t N,
          typename Indices = PONDER__SEQNS::make_index_sequence<N> >
R a2t(const std::array<T, N>& a)
{
    return a2t_impl<R>(a, Indices());
}

TEST_CASE("Check Ponder utilities work correctly")
{
    SECTION("integer_sequence")
    {
        auto is = PONDER__SEQNS::make_index_sequence<3>();
        REQUIRE(is.size() == 3);

        std::array<int, 4> array {{1,2,3,4}};

        // convert an array into a tuple
        auto tuple = a2t<std::tuple<int, int, int, int>>(array);
        static_assert(std::is_same<decltype(tuple), std::tuple<int, int, int, int>>::value, "");
    }

    SECTION("allTrue")
    {
        REQUIRE(ponder::detail::allTrue() == true);
        REQUIRE(ponder::detail::allTrue(true) == true);
        REQUIRE(ponder::detail::allTrue(true, true) == true);
        REQUIRE(ponder::detail::allTrue(true, true, true) == true);
        REQUIRE(ponder::detail::allTrue(true, true, true, true) == true);
        REQUIRE(ponder::detail::allTrue(true, true, true, true, true) == true);
        REQUIRE(ponder::detail::allTrue(true, true, true, true, true, true) == true);
        REQUIRE(ponder::detail::allTrue(true, true, true, true, true, true, true) == true);
        REQUIRE(ponder::detail::allTrue(true, true, true, true, true, true, true, true) == true);

        REQUIRE(ponder::detail::allTrue(false) == false);
        REQUIRE(ponder::detail::allTrue(true, false) == false);
        REQUIRE(ponder::detail::allTrue(true, false, true) == false);
        REQUIRE(ponder::detail::allTrue(true, true, false, true) == false);
        REQUIRE(ponder::detail::allTrue(true, true, true, false, true) == false);
        REQUIRE(ponder::detail::allTrue(true, false, true, true, true, true) == false);
        REQUIRE(ponder::detail::allTrue(true, true, false, true, true, true, true) == false);
        REQUIRE(ponder::detail::allTrue(true, false, true, true, true, false, true, true) == false);
    }

    SECTION("type to string")
    {
        REQUIRE(strcmp(ponder::detail::valueTypeAsString(ponder::ValueKind::None), "none")==0);
        REQUIRE(strcmp(ponder::detail::valueTypeAsString(ponder::ValueKind::Real), "real")==0);
        REQUIRE(strcmp(ponder::detail::valueTypeAsString(ponder::ValueKind::User), "user")==0);
    }
}

//----------------------------------------------------------------------------------------

TEST_CASE("Check IdTraits")
{
    SECTION("cstr")
    {
        const char *t1 = "flibaddydib";
        ponder::Id id(t1);
        ponder::IdRef ir(id);

        REQUIRE(strcmp(ponder::id::c_str(ir), t1) == 0);
    }
}

//----------------------------------------------------------------------------------------

#ifdef TEST_BOOST
// This library used to use Boost. These checks are too make sure the functionality
// we rely on is the same as the one replaced.
TEST_CASE("Check functionality same as Boost")
{
    SECTION("check traits same as Boost")
    {
        typedef void (*fn1_t)(void);

        static_assert(std::is_same<void(),
                      boost::function_types::function_type<fn1_t>::type>::value,
                      "boost::function_types problem");

        static_assert(std::is_same<void(), ponder::detail::FunctionTraits<fn1_t>::type>::value,
                      "ponder::detail::FunctionTraits problem");

        typedef int (*fn2_t)(int,const char*,float&);

        static_assert(std::is_same<int(int,const char*,float&),
                      boost::function_types::function_type<fn2_t>::type>::value,
                      "boost::function_types problem");
        static_assert(std::is_same<int(int,const char*,float&),
                      ponder::detail::FunctionTraits<fn2_t>::type>::value,
                      "ponder::detail::FunctionTraits problem");

        struct TestClass {
            int foo(float) {return 0;}
        };

        typedef int (TestClass::*fn3_t)(float);

        static_assert(std::is_same<int(TestClass&,float),
                      boost::function_types::function_type<fn3_t>::type>::value,
                      "boost::function_types problem");

        static_assert(std::is_same<int(TestClass&,float),
                      ponder::detail::MethodDetails<fn3_t>::FunctionKind>::value,
                      "ponder::detail::MethodDetails problem");

        static_assert(std::is_same<int(TestClass&,float),
                      ponder::detail::FunctionTraits<fn3_t>::type>::value,
                      "ponder::detail::FunctionTraits problem");
    }

    SECTION("boost_function")
    {
        typedef void (*fn1_t)(void);
        static_assert(
            std::is_same<void(), boost::function_types::function_type<fn1_t>::type>::value,
            "boost::function_types problem");

        typedef int (*fn2_t)(int,const char*,float&);
        static_assert(std::is_same<int(int,const char*,float&),
             boost::function_types::function_type<fn2_t>::type>::value,
             "boost::function_types problem");

        struct TestClass {
            int foo(float) {return 0;}
        };

        typedef int (TestClass::*fn3_t)(float);
        static_assert(std::is_same<int(TestClass&,float),
             boost::function_types::function_type<fn3_t>::type>::value,
             "boost::function_types problem");
    }

    SECTION("boost_callable")
    {
        static_assert( ! boost::function_types::is_callable_builtin<void>::value,
                      "boost::callable problem");
        static_assert( ! boost::function_types::is_callable_builtin<int>::value,
                      "boost::callable problem");
        static_assert( ! boost::function_types::is_callable_builtin<char*>::value,
                      "boost::callable problem");

        struct TestClass {
            int foo(float) {return 0;}
            int i;
            int arr[5];
        };
        static_assert(boost::function_types::is_callable_builtin<void()>::value,
                      "boost::callable problem");
        static_assert(boost::function_types::is_callable_builtin<int(TestClass::*)(float)>::value,
                      "boost::callable problem");

        static_assert(boost::function_types::is_callable_builtin<int(TestClass::*)>::value,
                      "boost::callable problem");
        static_assert(boost::function_types::is_callable_builtin<int(TestClass::*)[5]>::value,
                      "boost::callable problem");
    }

    SECTION("boost_result_type")
    {
        struct TestClass {
            int foo(float) {return 0;}
        };
        static_assert(
                  std::is_same<int, boost::function_types::result_type<int()>::type>::value,
                  "boost::ret result_type");

        static_assert(
            std::is_same<int,
                         boost::function_types::result_type<int(TestClass::*)(void)>::type>::value,
                      "boost::ret result_type");

        static_assert(
            std::is_same<float,
                    boost::function_types::result_type<float(TestClass::*)(void)>::type>::value,
                    "boost::ret result_type");
    }
}

#endif // TEST_BOOST

//----------------------------------------------------------------------------------------



