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

#ifndef CAMPTEST_PROPERTY_HPP
#define CAMPTEST_PROPERTY_HPP

#include <camp/camptype.hpp>
#include <camp/class.hpp>
#include <camp/enum.hpp>
#include <boost/shared_ptr.hpp>
#include <string>

namespace PropertyTest
{
    enum MyEnum
    {
        Zero = 0,
        One  = 1,
        Two  = 2
    };

    struct MyType
    {
        MyType(int x_) : x(x_) {}
        int x;
    };

    bool operator==(const MyType& left, const MyType& right) {return left.x == right.x;}
    bool operator<(const MyType& left, const MyType& right) {return left.x < right.x;}
    std::ostream& operator<<(std::ostream& stream, const MyType& object) {return stream << object.x;}

    struct MyClass
    {
        MyClass()
            : p1(true)
            , p2(2)
            , p3("3")
            , p4(MyType(4))
            , p5(false)
            , p6(6)
            , p7_impl("7"), p7(&p7_impl)
            , p8_impl(One), p8(&p8_impl)
            , p9(new MyType(9))
            , p10(true)
            , p11(11)
            , p12("12")
            , p13(Two)
            , p18(true)
            , p19(19)
            , p20(20.)
            , p21(21)
            , p22(Two)
            , p24(MyType(24))
        {
        }

        // ***** properties used in non-member functions *****
        bool p1;
        int p2;
        std::string p3;
        MyType p4;

        // ***** properties used as direct pointer to members *****
        bool p5;
        const int p6;
        std::string p7_impl; std::string* p7;
        MyEnum p8_impl; const MyEnum* p8;
        boost::shared_ptr<MyType> p9;

        // ***** member functions *****
        bool p10; bool getP10() {return p10;}
        int p11; int getP11() const {return p11;}
        std::string p12; std::string& getP12() {return p12;}
        MyEnum p13; MyEnum getP13() const {return p13;} void setP13(MyEnum e) {p13 = e;}

        // ***** nested properties *****
        struct Inner
        {
            Inner() : p14(false), p15(15), p16("16"), p17(MyType(17)) {}
            bool p14;
            const int p15;
            std::string p16; std::string getP16() const {return p16;}
            MyType p17; const MyType& getP17() const {return p17;} void setP17(MyType t) {p17 = t;}
        };
        Inner inner;
        Inner& getInner() {return inner;}

        // ***** properties used with boost::function *****
        bool p18;
        int p19; int& getP19() {return p19;}
        double p20; double getP20() const {return p20;} void setP20(double d) {p20 = d;}

        // ***** properties used with boost::bind *****
        int p21;
        MyEnum p22; MyEnum* getP22() {return &p22;}
        std::string getP23(const std::string& str) const {return str + "23";}
        MyType p24; const MyType& getP24() const {return p24;} void setP24(MyType t) {p24 = t;}
    };

    bool getP1(const MyClass& object) {return object.p1;}

    const int& getP2(MyClass& object) {return object.p2;}

    std::string& getP3(MyClass& object) {return object.p3;}

    const MyType& getP4(const MyClass& object) {return object.p4;}
    void setP4(MyClass& object, MyType value) {object.p4 = value;}

    int& getP21(MyClass& object) {return object.p21;}

    void declare()
    {
        camp::Enum::declare<MyEnum>("PropertyTest::MyEnum")
            .value("Zero", Zero)
            .value("One",  One)
            .value("Two",  Two);

        camp::Class::declare<MyType>("PropertyTest::MyType");

        camp::Class::declare<MyClass>("PropertyTest::MyClass")

            // ***** non-member functions *****
            .property("p1", &getP1)         // read-only getter (const param)
            .property("p2", &getP2)         // read-only getter (const return)
            .property("p3", &getP3)         // read-write getter
            .property("p4", &getP4, &setP4) // getter + setter

            // ***** pointer to members *****
            .property("p5", &MyClass::p5) // pointer to read-write member
            .property("p6", &MyClass::p6) // pointer to const member
            // TOFIX .property("p7", &MyClass::p7) // pointer to read-write pointer member
            // TOFIX .property("p8", &MyClass::p8) // pointer to const pointer member
            .property("p9", &MyClass::p9) // pointer to read-write smart pointer member

            // ***** members functions *****
            .property("p10", &MyClass::getP10)                   // read-only getter (return by value)
            .property("p11", &MyClass::getP11)                   // read-only getter (const)
            .property("p12", &MyClass::getP12)                   // read-write getter
            .property("p13", &MyClass::getP13, &MyClass::setP13) // read-only getter + write-only setter

            // ***** nested functions *****
            .property("p14", &MyClass::Inner::p14, &MyClass::getInner)    // pointer to read-write member
            .property("p15", &MyClass::Inner::p15, &MyClass::inner)       // Pointer to read-only member
            .property("p16", &MyClass::Inner::getP16, &MyClass::getInner) // read-only getter
            .property("p17", &MyClass::Inner::getP17,
                             &MyClass::Inner::setP17, &MyClass::inner)    // read-only getter + write-only setter

            // ***** boost::function *****
            .property("p18", boost::function<bool (MyClass&)>(&MyClass::p18))    // pointer to read-write member
            .property("p19", boost::function<int& (MyClass&)>(&MyClass::getP19)) // read-write getter
            .property("p20", boost::function<double (MyClass&)>(&MyClass::getP20),
                             boost::function<void (MyClass&, double)>(&MyClass::setP20)) // read-only getter + write-only setter

            // ***** boost::bind *****
            .property("p21", boost::bind(&getP21, _1))                 // non-member read-write getter
            // TOFIX .property("p22", boost::bind(&MyClass::getP22, _1))        // read-write getter to pointer
            .property("p23", boost::bind(&MyClass::getP23, _1, "str")) // read-only getter + extra parameter
            .property("p24", boost::bind(&MyClass::getP24, _1),
                             boost::bind(&MyClass::setP24, _1, _2))    // read-only getter + write-only setter
            ;
    }
}

CAMP_AUTO_TYPE(PropertyTest::MyEnum,  &PropertyTest::declare);
CAMP_AUTO_TYPE(PropertyTest::MyType,  &PropertyTest::declare);
CAMP_AUTO_TYPE(PropertyTest::MyClass, &PropertyTest::declare);

#endif // CAMPTEST_PROPERTY_HPP
