TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TEMPLATE = lib
CONFIG += staticlib

INCLUDEPATH += ../UsageEnvironment/include ../groupsock/include

target.path = $$PWD/../lib
INSTALLS += target

SOURCES += \
    UsageEnvironment.cpp \
    strDup.cpp \
    HashTable.cpp

HEADERS += \
    include/UsageEnvironment_version.hh \
    include/UsageEnvironment.hh \
    include/strDup.hh \
    include/HashTable.hh \
    include/Boolean.hh

