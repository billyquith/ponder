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


#ifndef TESTCLASS_HPP
#define TESTCLASS_HPP


#include <camp/camptype.hpp>
#include <camp/class.hpp>
#include <camp/enum.hpp>
#include <camp/value.hpp>
#include <camp/function.hpp>
#include <camp/property.hpp>
#include <camp/arrayproperty.hpp>
#include <camp/enumobject.hpp>
#include <camp/userobject.hpp>
#include <camp/classvisitor.hpp>
#include <camp/simpleproperty.hpp>
#include <camp/arrayproperty.hpp>
#include <camp/enumproperty.hpp>
#include <camp/userproperty.hpp>
#include <camp/observer.hpp>
#include <boost/shared_ptr.hpp>
#include <string>


class DummyClass1 {};
CAMP_TYPE(DummyClass1);
class DummyClass2 {};
CAMP_TYPE(DummyClass2);
class DummyClass3 {};
CAMP_TYPE(DummyClass3);
class DummyClass4 {};
CAMP_TYPE(DummyClass4);

enum DummyEnum1 {};
CAMP_TYPE(DummyEnum1);
enum DummyEnum2 {};
CAMP_TYPE(DummyEnum2);
enum DummyEnum3 {};
CAMP_TYPE(DummyEnum3);
enum DummyEnum4 {};
CAMP_TYPE(DummyEnum4);


struct AbstractClass
{
    virtual void f() = 0;
};
CAMP_TYPE(AbstractClass);

struct ConcreteClass : public AbstractClass
{
    virtual void f() {}
};
CAMP_TYPE(ConcreteClass);


struct TestClass
{
    CAMP_RTTI();

public:

    struct Nested
    {
    };
};
CAMP_TYPE(TestClass);
CAMP_TYPE(TestClass::Nested);


struct TestClass2
{
    CAMP_RTTI();
};
CAMP_TYPE(TestClass2);


struct DerivedClassNoRTTI : public TestClass
{
};
CAMP_TYPE(DerivedClassNoRTTI);


struct DerivedClassRTTI : public TestClass
{
    CAMP_RTTI();
};
CAMP_TYPE(DerivedClassRTTI);


struct DerivedMultiple : public TestClass, public TestClass2
{
    CAMP_RTTI();
};
CAMP_TYPE(DerivedMultiple);


struct DerivedDeep : public DerivedMultiple
{
    CAMP_RTTI();
};
CAMP_TYPE(DerivedDeep);

struct ObjectTest
{
    ObjectTest() : prop(5) {}
    std::string func(int x) {return boost::lexical_cast<std::string>(x);}
    int prop;
};
CAMP_TYPE(ObjectTest);

struct ComparableBase
{
    virtual ~ComparableBase() {}
    char padding[4];

    CAMP_RTTI()
};
CAMP_TYPE(ComparableBase);

struct Comparable : public ComparableBase
{
    Comparable(int x = -1) : m_x(x) {}
    bool operator ==(const Comparable& c) const {return m_x == c.m_x;}
    bool operator !=(const Comparable& c) const {return m_x != c.m_x;}
    int m_x;

    CAMP_RTTI()
};
CAMP_TYPE(Comparable);

std::ostream& operator <<(std::ostream& stream, const Comparable& c)
{
    return stream << c.m_x;
}


struct Owner
{
    Comparable get() const {return m_c;}
    void set(Comparable c) {m_c = c;}

    Comparable m_c;
};
CAMP_TYPE(Owner);

struct SuperOwner
{
    SuperOwner() : m_x(0) {}

    const Owner& get() const {return m_o;}
    void set(const Owner& o)
    {
        if (m_o.get() != o.get())
        {
            m_o = o;
            m_x++;
        }
    }

    Comparable getValue() const {return m_o.get();}
    void setValue(Comparable c) {m_o.set(c);}

    Owner m_o;
    int m_x;
};
CAMP_TYPE(SuperOwner);


enum TestEnum
{
    One = 1,
    Zero = 0,
    Ten = 10,
    MinusTwo = -2,
    Five = 5
};
CAMP_TYPE(TestEnum);


class TestString
{
public:

    TestString(const char* str = "") : m_str(str) {}

    const char* str() const {return m_str.c_str();}

    TestString SubString(std::size_t begin, std::size_t count) const {return m_str.substr(begin, count).c_str();}

    bool operator ==(const TestString& str) const {return m_str == str.m_str;}

private:

    std::string m_str;
};

std::ostream& operator <<(std::ostream& stream, const TestString& str)
{
    return stream << str.str();
}

namespace camp_ext
{
    template <>
    struct ValueMapper<TestString>
    {
        static const int type = camp::stringType;

        static std::string to(const TestString& source)
        {
            return source.str();
        }

        template <typename T>
        static TestString from(const T& source)
        {
            return ValueMapper<std::string>::from(source).c_str();
        }
    };
}

template <typename T>
struct SmartPointer
{
    SmartPointer(T* t) : m_t(t) {}

    T* operator->() const {return m_t.operator->();}
    boost::shared_ptr<T> m_t;
};


struct PropertyTest
{
    PropertyTest() : p0("hello"), p1(false), p2_(1), p3(new Comparable(33)), n2(new Nested) {}

    std::string p0;
    bool p1;
    short p2_;
    short getP2() const {return p2_;}
    void setP2(const short& x) {p2_ = x;}
    SmartPointer<Comparable> p3;

    struct Nested
    {
        Nested() : p4(Ten), p5(4) {}
        TestEnum p4;

        Comparable p5;
        const Comparable& getP5() const {return p5;}
        void setP5(const Comparable& c) {p5 = c;}
    };

    Nested n1;
    boost::shared_ptr<Nested> n2;
    Nested& n3() {return n1;}
};
CAMP_TYPE(PropertyTest);

std::string& PropertyTest_ref(PropertyTest& p)
{
    return p.p0;
}
std::string PropertyTest_get(PropertyTest& p)
{
    return p.p0;
}
void PropertyTest_set(PropertyTest& p, std::string s)
{
    p.p0 = s;
}


struct MyArray
{
    std::vector<std::string> v;
};

namespace camp_ext
{
template <>
struct ArrayMapper<MyArray>
{
    typedef std::string ElementType;

    static bool dynamic()
    {
        return true;
    }

    static std::size_t size(const MyArray& arr)
    {
        return arr.v.size();
    }

    static const std::string& get(const MyArray& arr, std::size_t index)
    {
        return arr.v[index];
    }

    static void set(MyArray& arr, std::size_t index, const std::string& value)
    {
        arr.v[index] = value;
    }

    static void insert(MyArray& arr, std::size_t before, const std::string& value)
    {
        arr.v.insert(arr.v.begin() + before, value);
    }

    static void remove(MyArray& arr, std::size_t index)
    {
        arr.v.erase(arr.v.begin() + index);
    }
};
}

struct ArrayTest
{
    ArrayTest()
    {
        a1[0] = true;
        a1[1] = false;
        a1[2] = true;

        a2[0] = One;
        a2[1] = Zero;
        a2[2] = Ten;
        a2[3] = MinusTwo;
        a2[4] = Five;

        a3.push_back(10);
        a3.push_back(20);

        a4.push_back(Comparable(100));
        a4.push_back(Comparable(200));
        a4.push_back(Comparable(300));
        a4.push_back(Comparable(400));

        a5.v.push_back("a5_0");
        a5.v.push_back("a5_1");
        a5.v.push_back("a5_2");
    }

    bool a1[3];
    boost::array<TestEnum, 5> a2;
    std::vector<int> a3;
    std::list<Comparable> a4;
    MyArray a5;
};
CAMP_TYPE(ArrayTest);


struct EnumPropTest
{
    EnumPropTest() : e(MinusTwo)
    {
    }

    TestEnum e;
};
CAMP_TYPE(EnumPropTest);


struct UserPropTest
{
    UserPropTest() : c1(10), c2(new Comparable(20)), c3(new Comparable(30)), c4(40)
    {
    }

    ~UserPropTest()
    {
        delete c2;
    }

    Comparable getC4() const {return c4;}
    void setC4(Comparable c) {c4 = c;}

    Comparable c1;
    Comparable* c2;
    boost::shared_ptr<Comparable> c3;
    Comparable c4;
};
CAMP_TYPE(UserPropTest);


struct FunctionTestBase
{
    std::string inheritedFunc() {return "inherited";}
    char padding[10];
};

struct FunctionTest : public FunctionTestBase, boost::noncopyable
{
    FunctionTest() :
        nestedPtr(&nested),
        nestedSmartPtr(new Nested),
        nestedMySmartPtr(new Nested)
    {
    }

    std::string s;
    float nonConstFunc() {return 5.5f;}
    bool constFunc() const {return false;}

    struct Nested
    {
        TestEnum func(int) const {return MinusTwo;}
    };
    Nested nested;
    const Nested& getNested() const {return nested;}
    Nested* nestedPtr;
    boost::shared_ptr<Nested> nestedSmartPtr;
    SmartPointer<Nested> nestedMySmartPtr;
    const Nested* getNestedPtr() const {return nestedPtr;}

    void f0() {}
    void f1(bool) {}
    void f2(float, double) {}
    void f3(short, int, long) {}
    void f4(const std::string&, std::string, const TestString&, TestString) {}
    void f5(TestEnum, TestEnum, TestEnum, TestEnum, TestEnum) {}

    camp::Value abstractFunc(camp::Value v) {return v;}

    ConcreteClass concrete;
    AbstractClass* ptrToAbstractRet() {return &concrete;}
    AbstractClass& refToAbstractRet() {return concrete;}
    const AbstractClass* ptrToAbstractConstRet() const {return &concrete;}
    const AbstractClass& refToAbstractConstRet() const {return concrete;}
    void ptrToAbstractArg(AbstractClass*) {}
    void refToAbstractArg(AbstractClass&) {}
    void ptrToAbstractConstArg(const AbstractClass*) {}
    void refToAbstractConstArg(const AbstractClass&) {}
};
CAMP_TYPE(FunctionTest);

void FunctionTest_byRef(FunctionTest& t)
{
    t.s = "hello";
}
int FunctionTest_byPtr(const FunctionTest*)
{
    return 24;
}

struct FunctorTest
{
    short operator()(short x) const {return x + 1;}
};


class TagTest
{
public:

    TagTest() : m_v("m_v") {}

    void f() {}
    int p;

    camp::Value m_v;
    camp::Value v1() {return "v1";}
    camp::Value v2() const {return "v2";}
};
CAMP_TYPE(TagTest);

camp::Value TagTest_FreeFunc(const TagTest& t)
{
    return t.m_v;
}


class PropertyReadableWritableTest
{
public:

    PropertyReadableWritableTest(bool b = true) : m_b(b) {}

    int p;

    bool m_b;
    bool b1() {return true;}
    bool b2() const {return false;}
};
CAMP_TYPE(PropertyReadableWritableTest);


class FunctionCallableTest
{
public:

    FunctionCallableTest(bool b = true) : m_b(b) {}

    void f() {}

    bool m_b;
    bool b1() {return true;}
    bool b2() const {return false;}
};
CAMP_TYPE(FunctionCallableTest);


struct ConstructorTestBase1
{
    ConstructorTestBase1() : str1("base1") {}
    virtual ~ConstructorTestBase1() {}
    std::string str1;
};
CAMP_TYPE(ConstructorTestBase1);

struct ConstructorTestBase2
{
    ConstructorTestBase2() : str2("base2") {}
    virtual ~ConstructorTestBase2() {}
    std::string str2;
};
CAMP_TYPE(ConstructorTestBase2);

struct ConstructorTest : public ConstructorTestBase1, public ConstructorTestBase2
{
    ConstructorTest()
        : args(0)
    {
    }

    ConstructorTest(bool b)
        : args(1)
        , arg1(b)
    {
    }

    ConstructorTest(bool b, int i) 
        : args(2)
        , arg1(b)
        , arg2(i)
    {
    }

    ConstructorTest(bool b, int i, const std::string& s)
        : args(3)
        , arg1(b)
        , arg2(i)
        , arg3(s)
    {
    }

    ConstructorTest(bool b, int i, const std::string& s, TestEnum t)
        : args(4)
        , arg1(b)
        , arg2(i)
        , arg3(s)
        , arg4(t)
    {
    }

    ConstructorTest(bool b, int i, const std::string& s, TestEnum t, Comparable c)
        : args(5)
        , arg1(b)
        , arg2(i)
        , arg3(s)
        , arg4(t)
        , arg5(c)
    {
    }

    int args;
    bool arg1;
    int arg2;
    std::string arg3;
    TestEnum arg4;
    Comparable arg5;
};
CAMP_TYPE(ConstructorTest);


struct Base1
{
    Base1() : p1(10) {}
    int p1;
    int f1() const {return 1;}
    CAMP_RTTI();
};
CAMP_TYPE(Base1);

struct Base2
{
    Base2() : p2(20) {}
    int p2;
    int f2() const {return 2;}
    virtual int fv() const {return p2;}
    CAMP_RTTI();
};
CAMP_TYPE(Base2);

struct Base3 : public Base1, public Base2
{
    Base3() : p3(30) {}
    int p3;
    int f3() const {return 3;}
    virtual int fv() const {return p3;}
    CAMP_RTTI();
};
CAMP_TYPE(Base3);

struct Base4 : public Base3
{
    Base4() : p4(40) {}
    int p4;
    int f4() const {return 4;}
    virtual int fv() const {return p4;}
    CAMP_RTTI();
};
CAMP_TYPE(Base4);


template <typename T>
class MyTestMapper
{
public:

    std::size_t propertyCount()
    {
        return T::propertyCount();
    }

    camp::Property* property(std::size_t index)
    {
        return new TestProperty(T::property(index));
    }

    std::size_t functionCount()
    {
        return T::functionCount();
    }

    camp::Function* function(std::size_t index)
    {
        return new TestFunction(T::function(index));
    }

private:

    class TestProperty : public camp::SimpleProperty
    {
    public:

        TestProperty(const std::string& name)
            : camp::SimpleProperty(name, camp::intType)
        {
        }

        virtual camp::Value getValue(const camp::UserObject& object) const
        {
            T& t = object.get<T>();
            return t.prop(name());
        }

        virtual void setValue(const camp::UserObject& object, const camp::Value& value) const
        {
            T& t = object.get<T>();
            t.prop(name()) = value.to<int>();
        }
    };

    class TestFunction : public camp::Function
    {
    public:

        TestFunction(const std::string& name)
            : camp::Function(name, camp::stringType)
        {
        }

        virtual camp::Value execute(const camp::UserObject& object, const camp::Args&) const
        {
            T& t = object.get<T>();
            return t.func(name());
        }
    };
};

struct MyTestMapping
{
    static std::size_t propertyCount() {return 5;}

    static std::string property(std::size_t index)
    {
        const char* names[] = {"prop0", "prop1", "prop2", "prop3", "prop4"};
        return names[index];
    }

    static std::size_t functionCount() {return 3;}

    static std::string function(std::size_t index)
    {
        const char* names[] = {"func0", "func1", "func2"};
        return names[index];
    }

    MyTestMapping()
    {
        m_props[property(0)] = 0;
        m_props[property(1)] = 10;
        m_props[property(2)] = 20;
        m_props[property(3)] = 30;
        m_props[property(4)] = 40;
    }

    int& prop(const std::string& name)
    {
        return m_props[name];
    }

    std::string func(const std::string& name)
    {
        return name + "_called";
    }

    std::map<std::string, int> m_props;
};
CAMP_TYPE(MyTestMapping);


struct ClassVisitorTest
{
    int simpleProp;
    std::string arrayProp[5];
    TestEnum enumProp;
    Comparable userProp;
    void function() {}
};
CAMP_TYPE(ClassVisitorTest);

class MyClassVisitor : public camp::ClassVisitor
{
public:

    MyClassVisitor() : count(0)
    {
    }

    virtual void visit(const camp::SimpleProperty& property)
    {
        count++;
        simpleName = property.name();
    }

    virtual void visit(const camp::ArrayProperty& property)
    {
        count++;
        arrayName = property.name();
    }

    virtual void visit(const camp::EnumProperty& property)
    {
        count++;
        enumName = property.name();
    }

    virtual void visit(const camp::UserProperty& property)
    {
        count++;
        userName = property.name();
    }

    virtual void visit(const camp::Function& function)
    {
        count++;
        functionName = function.name();
    }

    unsigned int count;
    std::string simpleName;
    std::string arrayName;
    std::string enumName;
    std::string userName;
    std::string functionName;
};


class TestObserver : public camp::Observer
{
public:

    TestObserver() : classes(0), enums(0) {}

    virtual void classAdded(const camp::Class&) {classes++;}
    virtual void classRemoved(const camp::Class&) {classes--;}
    virtual void enumAdded(const camp::Enum&) {enums++;}
    virtual void enumRemoved(const camp::Enum&) {enums--;}

    int classes;
    int enums;
};

#endif // TESTCLASS_HPP
