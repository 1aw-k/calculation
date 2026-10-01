QT += core gui widgets testlib

CONFIG += c++11 console testcase
CONFIG -= app_bundle

TEMPLATE = app
TARGET = calculator_ui_input_test

INCLUDEPATH += ..

SOURCES += \
    calculator_ui_input_test.cpp \
    ../calculator_engine.cpp \
    ../mainwindow.cpp

HEADERS += \
    calculator_ui_input_test.h \
    ../calculator_engine.h \
    ../mainwindow.h

FORMS += \
    ../mainwindow.ui
