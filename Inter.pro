TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    Lekser.cpp \
    Source.cpp \
    Token.cpp \
    treeelement.cpp \
    parser.cpp \
    variable.cpp \
    citymap.cpp \
    street.cpp \
    int.cpp \
    double.cpp \
    string.cpp \
    function.cpp \
    interpreter.cpp

HEADERS += \
    Lekser.h \
    Source.h \
    Token.h \
    treeelement.h \
    parser.h \
    variable.h \
    citymap.h \
    street.h \
    int.h \
    double.h \
    string.h \
    function.h \
    interpreter.h

DISTFILES += \
    zrodlo.txt
