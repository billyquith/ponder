TEMPLATE = lib
include(../config.pro)
VERSION = 0.1
CONFIG += exceptions \
    stl
CONFIG(debug, debug|release):TARGET = campd
else:TARGET = camp
DEFINES += CAMP_EXPORTS
INCLUDEPATH += ./include \
    $${BOOST_PATH}
SOURCES += ./src/userproperty.cpp \
    ./src/userobject.cpp \
    ./src/tagholder.cpp \
    ./src/simpleproperty.cpp \
    ./src/property.cpp \
    ./src/observer.cpp \
    ./src/invalidvalue.cpp \
    ./src/invalidproperty.cpp \
    ./src/invalidobject.cpp \
    ./src/invalidindex.cpp \
    ./src/invalidfunction.cpp \
    ./src/invalidenumvalue.cpp \
    ./src/invalidenumname.cpp \
    ./src/invalidenum.cpp \
    ./src/invalidclass.cpp \
    ./src/invalidargument.cpp \
    ./src/invalidaccess.cpp \
    ./src/function.cpp \
    ./src/error.cpp \
    ./src/enumproperty.cpp \
    ./src/enumobject.cpp \
    ./src/enumbuilder.cpp \
    ./src/enum.cpp \
    ./src/classvisitor.cpp \
    ./src/class.cpp \
    ./src/arrayproperty.cpp \
    ./src/args.cpp \
    ./src/value.cpp \
    ./src/classmanager.cpp \
    ./src/enummanager.cpp \
    ./src/observernotifier.cpp \
    ./src/camptype.cpp \
    ./src/objectholderbyprop.cpp \
    ./src/classcast.cpp
OTHER_FILES += ./include/camp/value.inl \
    ./include/camp/userobject.inl \
    ./include/camp/getter.inl \
    ./include/camp/enumobject.inl \
    ./include/camp/enumget.inl \
    ./include/camp/enum.inl \
    ./include/camp/classget.inl \
    ./include/camp/classbuilder.inl \
    ./include/camp/class.inl \
    ./include/camp/detail/userpropertyimpl.inl \
    ./include/camp/detail/simplepropertyimpl.inl \
    ./include/camp/detail/getter.inl \
    ./include/camp/detail/enumpropertyimpl.inl \
    ./include/camp/detail/arraypropertyimpl.inl \
    ./include/camp/detail/objectholder.inl
HEADERS += ./include/camp/valuemapper.hpp \
    ./include/camp/value.hpp \
    ./include/camp/userproperty.hpp \
    ./include/camp/userobject.hpp \
    ./include/camp/type.hpp \
    ./include/camp/tagholder.hpp \
    ./include/camp/simpleproperty.hpp \
    ./include/camp/property.hpp \
    ./include/camp/observer.hpp \
    ./include/camp/invalidvalue.hpp \
    ./include/camp/invalidproperty.hpp \
    ./include/camp/invalidobject.hpp \
    ./include/camp/invalidindex.hpp \
    ./include/camp/invalidfunction.hpp \
    ./include/camp/invalidenumvalue.hpp \
    ./include/camp/invalidenumname.hpp \
    ./include/camp/invalidenum.hpp \
    ./include/camp/invalidclass.hpp \
    ./include/camp/invalidargument.hpp \
    ./include/camp/invalidaccess.hpp \
    ./include/camp/getter.hpp \
    ./include/camp/function.hpp \
    ./include/camp/error.hpp \
    ./include/camp/enumproperty.hpp \
    ./include/camp/enumobject.hpp \
    ./include/camp/enumget.hpp \
    ./include/camp/enumbuilder.hpp \
    ./include/camp/enum.hpp \
    ./include/camp/constructor.hpp \
    ./include/camp/config.hpp \
    ./include/camp/classvisitor.hpp \
    ./include/camp/classget.hpp \
    ./include/camp/classbuilder.hpp \
    ./include/camp/class.hpp \
    ./include/camp/camptype.hpp \
    ./include/camp/arrayproperty.hpp \
    ./include/camp/arraymapper.hpp \
    ./include/camp/args.hpp \
    ./include/camp/valuevisitor.hpp \
    ./include/camp/detail/valueimpl.hpp \
    ./include/camp/detail/userpropertyimpl.hpp \
    ./include/camp/detail/typeid.hpp \
    ./include/camp/detail/simplepropertyimpl.hpp \
    ./include/camp/detail/returntype.hpp \
    ./include/camp/detail/rawtype.hpp \
    ./include/camp/detail/propertyfactory.hpp \
    ./include/camp/detail/objecttraits.hpp \
    ./include/camp/detail/issmartpointer.hpp \
    ./include/camp/detail/getter.hpp \
    ./include/camp/detail/functiontraits.hpp \
    ./include/camp/detail/functionimpl.hpp \
    ./include/camp/detail/enumpropertyimpl.hpp \
    ./include/camp/detail/enummanager.hpp \
    ./include/camp/detail/constructorimpl.hpp \
    ./include/camp/detail/classmanager.hpp \
    ./include/camp/detail/callhelper.hpp \
    ./include/camp/detail/arraypropertyimpl.hpp \
    ./include/camp/detail/yesnotype.hpp \
    ./include/camp/detail/observernotifier.hpp \
    ./include/camp/qt/qtvariantconverter.hpp \
    ./include/camp/qt/qtsimpleproperty.hpp \
    ./include/camp/qt/qtmapper.hpp \
    ./include/camp/qt/qt.hpp \
    ./include/camp/qt/qstring.hpp \
    ./include/camp/qt/qlist.hpp \
    ./include/camp/qt/qvector.hpp \
    ./include/camp/detail/objectholder.hpp \
    ./include/camp/detail/objectholderbyprop.hpp \
    ./include/camp/classcast.hpp
