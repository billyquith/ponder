TEMPLATE = app

include(../../common/build/config.pro)

CONFIG -= qt

CONFIG += exceptions stl console

CONFIG(debug, debug|release) {
    OBJECTS_DIR = debug
    TARGET = camptestd
    win32:LIBS += -lcampd0 -lutfd
    unix:LIBS += -lcampd -lutfd
} else {
    OBJECTS_DIR = release
    TARGET = camptest
    win32:LIBS += -lcamp0 -lutf
    unix:LIBS += -lcamp -lutf
}

DESTDIR = ./
SOURCES += main.cpp
HEADERS += test.hpp

INCLUDEPATH += ../include $${BOOST_PATH}
LIBS += -L../lib/ -L../../common/utf/lib
