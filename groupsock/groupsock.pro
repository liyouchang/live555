TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TEMPLATE = lib
CONFIG += staticlib


INCLUDEPATH += ../groupsock/include ../UsageEnvironment/include

DEFINES += SOCKLEN_T=socklen_t

target.path = $$PWD/../lib
INSTALLS += target

SOURCES += \
    NetInterface.cpp \
    NetAddress.cpp \
    IOHandlers.cpp \
    GroupsockHelper.cpp \
    Groupsock.cpp \
    GroupEId.cpp \
    inet.c

HEADERS += \
    include/TunnelEncaps.hh \
    include/NetInterface.hh \
    include/NetCommon.h \
    include/NetAddress.hh \
    include/IOHandlers.hh \
    include/groupsock_version.hh \
    include/GroupsockHelper.hh \
    include/Groupsock.hh \
    include/GroupEId.hh

