QT += testlib
QT -= gui
QT += network
QT += xml

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_tests.cpp \
    functions.cpp

HEADERS += \
    functions.h
