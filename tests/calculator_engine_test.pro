QT += core testlib
QT -= gui

CONFIG += c++11 console testcase
CONFIG -= app_bundle

TEMPLATE = app
TARGET = calculator_engine_test

INCLUDEPATH += ..

SOURCES += \
    calculator_engine_test.cpp \
    ../calculator_engine.cpp

HEADERS += \
    calculator_engine_test.h \
    ../calculator_engine.h
