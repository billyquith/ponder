/****************************************************************************
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Copyright (C) 2015-2019 Nick Trout.
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

// Tests for exposing different class variations:
//  - Solo class/struct, static and dynamic inheritance.
//  - Downcasting, polymorphism.
//  - Property accessing.
//  - Get class by name/type.
//  - Undeclare types.

#include <ponder/classbuilder.hpp>
#include "test.hpp"

// See notes on the tests below.
#define TEST_VIRTUAL 0

namespace ClassTest
{
    struct MyExplicityDeclaredClass
    {
    };
    
    struct MyUndeclaredClass
    {
    };
    
    struct MyClass
    {
        void func() {}
        int prop;
    };
    
    struct MyClass2
    {
    };
    
    struct Base
    {
        virtual ~Base() {}
        PONDER_POLYMORPHIC();
    };
    
    struct Derived : Base
    {
        PONDER_POLYMORPHIC();
    };
    
    struct DerivedNoRtti : Base
    {
    };
    
    struct Derived2NoRtti : Derived
    {
    };
    
    struct TemporaryRegistration : Base
    {
        int a, b;
    };
    
    struct DifferentName
    {
    };
    static constexpr char c_differentName[] = "HatstandEscalator";
    
    template <class T>
    class TemplateClass
    {
    public:
        T testMember_;
    };
    
    // multi-argument template
    template <typename T, int X, int Y>
    struct DataTemplate
    {
        T data[Y][X];
        
        T get(int x, int y) const { return data[y][x]; }
        void set(int x, int y, T value) { data[y][x] = value; }
    };

    // See notes on the tests below.
    class VirtualBase
    {
    public:
        int a;
    };
    
    class VirtualX : public virtual VirtualBase
    {
    };

    class VirtualY : public virtual VirtualBase
    {
    };

    class VirtualZ : public VirtualBase
    {
    };
    
    class VirtualUser : public VirtualX, public VirtualY, public VirtualZ
    {
    };
    
    void declare()
    {
        ponder::Class::declare<MyClass>()
            .property("prop", &MyClass::prop)
            .function("func", &MyClass::func);
        
        ponder::Class::declare<MyClass2>();
        
        ponder::Class::declare<Base>();
        
        ponder::Class::declare<Derived>()
            .base<Base>();
        
        ponder::Class::declare<DerivedNoRtti>()
            .base<Base>();
        
        ponder::Class::declare<Derived2NoRtti>()
            .base<Derived>();
        
        ponder::Class::declare<TemplateClass<int>>()
            .constructor()
            .property("TestMember", &TemplateClass<int>::testMember_);

        ponder::Class::declare< DataTemplate<float,5,5> >()
            .function("get", &DataTemplate<float,5,5>::get)
            .function("set", &DataTemplate<float,5,5>::set);

        ponder::Class::declare<DifferentName>(c_differentName);

#if TEST_VIRTUAL
        ponder::Class::declare< VirtualBase >()
            ;

        ponder::Class::declare< VirtualX >()
            .base< VirtualBase >()
            ;

        ponder::Class::declare< VirtualY >()
            .base< VirtualBase >()
            ;

        ponder::Class::declare< VirtualZ >()
            .base< VirtualBase >()
            ;
        
        ponder::Class::declare< VirtualUser >()
            .base< VirtualX >()
            .base< VirtualY >()
            .base< VirtualZ >()
            ;
#endif
    }
    
    void declare_temp()
    {
        ponder::Class::declare<TemporaryRegistration>()
            .base<Base>()
            .property("a", &TemporaryRegistration::a)
            .property("b", &TemporaryRegistration::b);
    }

    void undeclare_temp()
    {
        ponder::Class::undeclare<TemporaryRegistration>();
    }
    
} // namespace ClassTest

PONDER_TYPE(ClassTest::MyExplicityDeclaredClass /* never declared */)
PONDER_TYPE(ClassTest::MyUndeclaredClass /* never declared */)
PONDER_TYPE(ClassTest::DataTemplate<float,5,5>)
PONDER_TYPE(ClassTest::DifferentName)

//
// ClassTest::declare() is called to declared registered classes in it. Note,
// function will only be called once, by the first class that is required. If
// re-registered (a duplicate), this would throw an exception.
//
PONDER_AUTO_TYPE(ClassTest::MyClass, &ClassTest::declare)
PONDER_AUTO_TYPE(ClassTest::MyClass2, &ClassTest::declare)
PONDER_AUTO_TYPE(ClassTest::Base, &ClassTest::declare)
PONDER_AUTO_TYPE(ClassTest::Derived, &ClassTest::declare)
PONDER_AUTO_TYPE(ClassTest::DerivedNoRtti, &ClassTest::declare)
PONDER_AUTO_TYPE(ClassTest::Derived2NoRtti, &ClassTest::declare)
PONDER_AUTO_TYPE(ClassTest::TemplateClass<int>, &ClassTest::declare)

PONDER_AUTO_TYPE(ClassTest::VirtualBase, &ClassTest::declare)
PONDER_AUTO_TYPE(ClassTest::VirtualX, &ClassTest::declare)
PONDER_AUTO_TYPE(ClassTest::VirtualY, &ClassTest::declare)
PONDER_AUTO_TYPE(ClassTest::VirtualZ, &ClassTest::declare)
PONDER_AUTO_TYPE(ClassTest::VirtualUser, &ClassTest::declare)

PONDER_TYPE(ClassTest::TemporaryRegistration);

using namespace ClassTest;

//-----------------------------------------------------------------------------
//                         Tests for ponder::Class
//-----------------------------------------------------------------------------

TEST_CASE("Classes need to be declared")
{
    SECTION("explicit declaration")
    {
        const std::size_t count = ponder::classCount();    
        ponder::Class::declare<MyExplicityDeclaredClass>();
        REQUIRE(ponder::classCount() == count + 1);
        REQUIRE(ponder::classByTypeSafe<MyExplicityDeclaredClass>() != nullptr);
    }
    
    SECTION("duplicates are errors")
    {
        ponder::classByType<MyClass>(); // to make sure it is declared
    
        // duplicate by type
        REQUIRE_THROWS_AS(ponder::Class::declare<MyClass>(), ponder::ClassAlreadyCreated);
        
        // duplicate by name
        REQUIRE_THROWS_AS(ponder::Class::declare<MyUndeclaredClass>("ClassTest::MyClass"),
                          ponder::ClassAlreadyCreated);        
    }
}


TEST_CASE("Class metadata can be retrieved")
{
    MyClass object;
    MyUndeclaredClass object2;    

    SECTION("by name that matches type")
    {
        REQUIRE(ponder::classByName("ClassTest::MyClass").name() == "ClassTest::MyClass");
        REQUIRE((ponder::classByName("ClassTest::MyClass") == ponder::classByType<ClassTest::MyClass>()));
    }

    SECTION("unfound names are errors")
    {
        REQUIRE_THROWS_AS(ponder::classByName("xThisWillNotBeFoundx"), ponder::ClassNotFound);
    }

    SECTION("by name that does not match type")
    {
        // lookup by static name fails
        REQUIRE_THROWS_AS(ponder::classByName("ClassTest::DifferentName"), ponder::ClassNotFound);

        // Lookup by declared name passes
        REQUIRE(ponder::classByName(c_differentName).name() == c_differentName);
        REQUIRE(ponder::classByType<ClassTest::DifferentName>().name() == c_differentName);
    }

    SECTION("by type")
    {
        REQUIRE(ponder::classByType<MyClass>().name() == "ClassTest::MyClass");
        
        REQUIRE(ponder::classByTypeSafe<MyUndeclaredClass>() == nullptr);
        
        REQUIRE_THROWS_AS(ponder::classByType<MyUndeclaredClass>(), ponder::ClassNotFound);
    }

    SECTION("by instance")
    {
        REQUIRE(ponder::classByObject(object).name() == "ClassTest::MyClass");
        REQUIRE(ponder::classByObject(&object).name() == "ClassTest::MyClass");
        
        REQUIRE_THROWS_AS(ponder::classByObject(object2), ponder::ClassNotFound);
        REQUIRE_THROWS_AS(ponder::classByObject(&object2), ponder::ClassNotFound);
    }
    
    SECTION("iteration")
    {
        size_t count = 0;
        for (auto&& cls : ponder::classIterator())
        {
            (void) cls.second->name();
            ++count;
        }
        REQUIRE(count == ponder::classCount());
    }
    
    SECTION("metadata can be compared")
    {
        const ponder::Class& class1 = ponder::classByType<MyClass>();
        const ponder::Class& class2 = ponder::classByType<MyClass2>();
        
        REQUIRE( (class1 == class1) );
        REQUIRE( (class1 != class2) );
        REQUIRE( (class2 != class1) );
    }
    
    SECTION("can retrieve memory size")
    {
        REQUIRE(ponder::classByType<MyClass>().sizeOf() == sizeof(MyClass));
    }
}


TEST_CASE("Class members can be inspected")
{
    const ponder::Class& metaclass = ponder::classByType<MyClass>();

    SECTION("can have properties")
    {
        REQUIRE(metaclass.propertyCount() == 1U);
        REQUIRE(metaclass.hasProperty("prop") == true);
        REQUIRE(metaclass.hasProperty("xxxx") == false);        
    }

    SECTION("can tryFind properties")
    {
        const ponder::Property *pp = nullptr;
        REQUIRE(metaclass.tryProperty("propNotFound", pp) == false);
        REQUIRE(metaclass.tryProperty("prop", pp) == true);
        REQUIRE(pp->name() == "prop");
    }

    SECTION("can have functions")
    {
        REQUIRE(metaclass.functionCount() == 1U);
        REQUIRE(metaclass.hasFunction("func") == true);
        REQUIRE(metaclass.hasFunction("xxxx") == false);
    }
    
    SECTION("can tryFind functions")
    {
        const ponder::Function *fp = nullptr;
        REQUIRE(metaclass.tryFunction("funcNotFound", fp) == false);
        REQUIRE(metaclass.tryFunction("func", fp) == true);
        REQUIRE(fp->name() == "func");
    }
    
    SECTION("can iterate over properties")
    {
        int index = 0;
        for (auto&& prop : metaclass.propertyIterator())
        {
            switch (index++) {
                case 0:
                    REQUIRE(prop.name() == ponder::String("prop"));
                    break;
                default:
                    ;
            }
        }
    }

    SECTION("can iterate over functions")
    {
        int index = 0;
        for (auto&& func : metaclass.functionIterator())
        {
            switch (index++) {
                case 0:
                    REQUIRE(func.name() == ponder::String("func"));
                    break;
                default:
                    ;
            }
        }
    }
}


TEST_CASE("Classes can use inheritance")
{
    const ponder::Class& derived = ponder::classByType<Derived>();

    REQUIRE(derived.baseCount() == 1U);
    REQUIRE(derived.base(0).name() == "ClassTest::Base");
    REQUIRE_THROWS_AS(derived.base(1), ponder::OutOfRange);
}


TEST_CASE("Classes can have hierarchies")
{
    Base* base    = new Base;
    Base* derived = new Derived;
    Base* nortti  = new DerivedNoRtti;
    Base* nortti2 = new Derived2NoRtti;

    REQUIRE(ponder::classByObject(*base).name() == "ClassTest::Base");
    
    SECTION("with rtti")
    {
        // Ponder finds its real type thanks to PONDER_POLYMORPHIC
        REQUIRE(ponder::classByObject(*derived).name() == "ClassTest::Derived");
    }
    
    SECTION("without rtti")
    {
        // Ponder fails to find its derived type without PONDER_POLYMORPHIC
        REQUIRE(ponder::classByObject(*nortti).name() == "ClassTest::Base");
    }

   SECTION("allows polymorphism")
   {
       Base* genericBase = derived;
       REQUIRE(ponder::classByObject(*genericBase).name() == "ClassTest::Derived");
   }
    
    SECTION("without rtti, no polymorphism")
    {
        Base* nonGenericBase = nortti;
        // Ponder fails to find its derived type without PONDER_POLYMORPHIC
        REQUIRE(ponder::classByObject(*nonGenericBase).name() == "ClassTest::Base");
    }

    // Ponder finds the closest derived type which has PONDER_POLYMORPHIC
    // REQUIRE(ponder::classByObject(nortti2).name(),  "ClassTest::Derived");
    // REQUIRE(ponder::classByObject(*nortti2).name(), "ClassTest::Derived");

    delete nortti2;
    delete nortti;
    delete derived;
    delete base;
}


TEST_CASE("Classes can by undeclared")
{
    SECTION("check before declaration")
    {
        REQUIRE_THROWS_AS(ponder::classByType<TemporaryRegistration>(), ponder::ClassNotFound);
    }

    SECTION("declare")
    {
        declare_temp();

        const ponder::Class& tempClass = ponder::classByType<TemporaryRegistration>();
    
        REQUIRE(tempClass.baseCount() == 1u);
        REQUIRE(tempClass.base(0).name() == "ClassTest::Base");
    }
    
    SECTION("do undeclare")
    {
        undeclare_temp();

        REQUIRE(ponder::classByTypeSafe<TemporaryRegistration>() == nullptr);
    }
}


TEST_CASE("Classes can be templates")
{
    auto const& metaclass = ponder::classByType< TemplateClass<int> >();

    REQUIRE(metaclass.constructorCount() > 0);
    REQUIRE(metaclass.propertyCount() > 0);
    
//    auto const& obj = metaclass.construct();
}

// Classes declared using Ponder *can declare* virtually inherited base classes,
// but the casting of these is unreliable due to the compiler specific nature of their
// implementation. This test is therefore optional as it fails on some
// platforms, even depending on config, e.g. Windows debug.
#if TEST_VIRTUAL
TEST_CASE("Classes can have virtual inhertitance")
{
    auto const& metaclass = ponder::classByType< VirtualUser >();
    
    auto const& baseX = metaclass.base(0);

    auto vuobj = metaclass.construct();
    
    auto vx = ponder::classCast(vuobj.pointer(), metaclass, baseX);
}
#endif


