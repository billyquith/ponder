cmake_minimum_required(VERSION 2.6)

# set project's name
project(CAMP)

# all .cpp files
set(CAMP_SRCS
    include/camp/userproperty.hpp
    src/userproperty.cpp
    include/camp/userobject.hpp
    include/camp/userobject.inl
    src/userobject.cpp
    include/camp/tagholder.hpp
    src/tagholder.cpp
    include/camp/simpleproperty.hpp
    src/simpleproperty.cpp
    include/camp/property.hpp
    src/property.cpp
    include/camp/observer.hpp
    src/observer.cpp
    include/camp/invalidvalue.hpp
    src/invalidvalue.cpp
    include/camp/invalidproperty.hpp
    src/invalidproperty.cpp
    include/camp/invalidobject.hpp
    src/invalidobject.cpp
    include/camp/invalidindex.hpp
    src/invalidindex.cpp
    include/camp/invalidfunction.hpp
    src/invalidfunction.cpp
    include/camp/invalidenumvalue.hpp
    src/invalidenumvalue.cpp
    include/camp/invalidenumname.hpp
    src/invalidenumname.cpp
    include/camp/invalidenum.hpp
    src/invalidenum.cpp
    include/camp/invalidclass.hpp
    src/invalidclass.cpp
    include/camp/invalidargument.hpp
    src/invalidargument.cpp
    include/camp/invalidaccess.hpp
    src/invalidaccess.cpp
    include/camp/function.hpp
    src/function.cpp
    include/camp/error.hpp
    src/error.cpp
    include/camp/enumproperty.hpp
    src/enumproperty.cpp
    include/camp/enumobject.hpp
    include/camp/enumobject.inl
    src/enumobject.cpp
    include/camp/enumbuilder.hpp
    src/enumbuilder.cpp
    include/camp/enum.hpp
    include/camp/enum.inl
    src/enum.cpp
    include/camp/classvisitor.hpp
    src/classvisitor.cpp
    include/camp/class.hpp
    include/camp/class.inl
    src/class.cpp
    include/camp/arrayproperty.hpp
    src/arrayproperty.cpp
    include/camp/args.hpp
    src/args.cpp
    include/camp/value.hpp
    include/camp/value.inl
    src/value.cpp
    include/camp/detail/classmanager.hpp
    src/classmanager.cpp
    include/camp/detail/enummanager.hpp
    src/enummanager.cpp
    include/camp/detail/observernotifier.hpp
    src/observernotifier.cpp
    include/camp/camptype.hpp
    src/camptype.cpp
    include/camp/classcast.hpp
    src/classcast.cpp
    include/camp/type.hpp
    include/camp/enumget.hpp
    include/camp/enumget.inl
    include/camp/constructor.hpp
    include/camp/config.hpp
    include/camp/classget.hpp
    include/camp/classget.inl
    include/camp/classbuilder.hpp
    include/camp/classbuilder.inl
    include/camp/arraymapper.hpp
    include/camp/valuemapper.hpp
    include/camp/valuevisitor.hpp
    include/camp/detail/valueimpl.hpp
    include/camp/detail/userpropertyimpl.hpp
    include/camp/detail/userpropertyimpl.inl
    include/camp/detail/typeid.hpp
    include/camp/detail/simplepropertyimpl.hpp
    include/camp/detail/simplepropertyimpl.inl
    include/camp/detail/returntype.hpp
    include/camp/detail/rawtype.hpp
    include/camp/detail/propertyfactory.hpp
    include/camp/detail/objecttraits.hpp
    include/camp/detail/issmartpointer.hpp
    include/camp/detail/getter.hpp
    include/camp/detail/getter.inl
    include/camp/detail/functiontraits.hpp
    include/camp/detail/functionimpl.hpp
    include/camp/detail/enumpropertyimpl.hpp
    include/camp/detail/enumpropertyimpl.inl
    include/camp/detail/constructorimpl.hpp
    include/camp/detail/callhelper.hpp
    include/camp/detail/arraypropertyimpl.hpp
    include/camp/detail/arraypropertyimpl.inl
    include/camp/detail/yesnotype.hpp
    include/camp/detail/objectholder.hpp
    include/camp/detail/objectholder.inl
    include/camp/qt/qtvariantconverter.hpp
    include/camp/qt/qtsimpleproperty.hpp
    include/camp/qt/qtmapper.hpp
    include/camp/qt/qt.hpp
    include/camp/qt/qstring.hpp
    include/camp/qt/qlist.hpp
    include/camp/qt/qvector.hpp
)

# find Boost
find_package(Boost 1.38.0 REQUIRED)

# include files search paths
include_directories(
    ${CAMP_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIRS}
)

# instruct CMake to build a shared library from all of the source files
add_library(camp SHARED ${CAMP_SRCS})

# define the export macro
set_target_properties(camp PROPERTIES DEFINE_SYMBOL CAMP_EXPORTS)

# add the test subdirectory, but do not build it by default
add_subdirectory(test EXCLUDE_FROM_ALL)

