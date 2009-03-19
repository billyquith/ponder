TEMPLATE = app

include(../../config.pro)

CONFIG += exceptions stl console

CONFIG(debug, debug|release) {
    TARGET = camptestd
    win32:LIBS += -lcampd0 -lutfd
    unix:LIBS += -lcampd -lutfd
} else {
    TARGET = camptest
    win32:LIBS += -lcamp0 -lutf
    unix:LIBS += -lcamp -lutf
}

SOURCES += main.cpp
HEADERS += test.hpp

INCLUDEPATH += ../include $${BOOST_PATH}
LIBS += -L../lib/ -L$${COMMON_PATH}/utf/lib
