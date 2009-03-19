TEMPLATE = app
include(../../../../../config.pro)

CONFIG += exceptions stl console qt

CONFIG(debug, debug|release) {
    TARGET = propertyeditord
    LIBS += -lcampd
} else {
    TARGET = propertyeditor
    LIBS += -lcamp
}

INCLUDEPATH += ../../../../include $${BOOST_PATH}
LIBS += -L../../../../lib

SOURCES += main.cpp \
    delegate.cpp \
    propertyeditor.cpp \
    propertynode.cpp \
    treemodel.cpp \
    visitor.cpp

HEADERS += declaration.hpp \
    delegate.hpp \
    propertyeditor.hpp \
    propertynode.hpp \
    treemodel.hpp \
    visitor.hpp

