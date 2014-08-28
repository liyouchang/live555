TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TEMPLATE = lib
CONFIG += staticlib

INCLUDEPATH += ../BasicUsageEnvironment/include ../groupsock/include ../UsageEnvironment/include

#message( ddd  $$OUT_PWD)

SOURCES += \
    DelayQueue.cpp \
    BasicUsageEnvironment0.cpp \
    BasicUsageEnvironment.cpp \
    BasicTaskScheduler0.cpp \
    BasicTaskScheduler.cpp \
    BasicHashTable.cpp

#target.files = $$DESTDIR/$$TARGET
target.path = $$PWD/../libs
INSTALLS += target

HEADERS += \
    include/HandlerSet.hh \
    include/DelayQueue.hh \
    include/BasicUsageEnvironment_version.hh \
    include/BasicUsageEnvironment0.hh \
    include/BasicUsageEnvironment.hh \
    include/BasicHashTable.hh
