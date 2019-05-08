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
    parser.cpp

HEADERS += \
    Lekser.h \
    Source.h \
    Token.h \
    treeelement.h \
    parser.h

DISTFILES += \
    zrodlo.txt
