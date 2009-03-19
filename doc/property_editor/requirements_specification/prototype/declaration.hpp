#ifndef DECLARATION_H
#define DECLARATION_H

#include <camp/camptype.hpp>

#include <QList>

#include <string>

class DummyClass
{
public:
    DummyClass() : m_dummyInt(0), m_dummyStr("default dummy string"), m_dummyBool(false)
    {
    }

    int m_dummyInt;
    std::string m_dummyStr;
    bool m_dummyBool;
    QList<int> m_dummyList;
};
CAMP_TYPE(DummyClass);

class BaseClass
{
    CAMP_RTTI();

public:
    BaseClass() : m_baseInt(1), m_baseStr("default base string")
    {
    }

    DummyClass m_baseDummy1;
    DummyClass m_baseDummy2;
    int m_baseInt;
    std::string m_baseStr;
};
CAMP_TYPE(BaseClass);

class SubClass : public BaseClass
{
    CAMP_RTTI();

public:
    SubClass() : m_subInt(2), m_subStr("default sub string")
    {
    }

    DummyClass m_subDummy;
    int m_subInt;
    std::string m_subStr;
};
CAMP_TYPE(SubClass);

#endif // DECLARATION_H

