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
}

CAMP_TYPE(PropertyTest::MyEnum);
CAMP_TYPE(PropertyTest::MyType);
CAMP_TYPE(PropertyTest::MyClass);

#endif // CAMPTEST_PROPERTY_HPP
