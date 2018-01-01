QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

SOURCES += main.cpp \
    controller.cpp

include($$PWD/libasteroid/libasteroid.pri)

HEADERS += \
    controller.h
