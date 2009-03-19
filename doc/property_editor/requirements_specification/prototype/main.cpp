
#include <declaration.hpp>
#include <propertyeditor.hpp>

#include <camp/class.hpp>
#include <camp/qt/qt.hpp>

#include <QApplication>

int main(int argc, char** argv)
{
    camp::Class::declare<DummyClass>("DummyClass")
        .property("dummyInt", &DummyClass::m_dummyInt)
        .property("dummyStr", &DummyClass::m_dummyStr)
        .property("dummyBool", &DummyClass::m_dummyBool)
        .property("dummyArray", &DummyClass::m_dummyList)
        ;

    camp::Class::declare<BaseClass>("BaseClass")
        .property("baseDummy1", &BaseClass::m_baseDummy1)
        .property("baseDummy2", &BaseClass::m_baseDummy2)
        .property("baseInt", &BaseClass::m_baseInt)
        .property("baseStr", &BaseClass::m_baseStr)
        ;

    camp::Class::declare<SubClass>("SubClass")
        .base<BaseClass>()
        .property("subDummy", &SubClass::m_subDummy)
        .property("subInt", &SubClass::m_subInt)
            .readable(false)
        .property("subStr", &SubClass::m_subStr)
            .writable(false)
        ;


    QApplication app(argc, argv);

    //DummyClass dummy1;
    SubClass sub1;
    sub1.m_subDummy.m_dummyList << 4 << 2 << 6;

    PropertyEditor editor;
    //editor.setSubject(dummy1);
    editor.setSubject(sub1);

    editor.show();
    editor.resize(400, 500);
    
    return app.exec();
}

