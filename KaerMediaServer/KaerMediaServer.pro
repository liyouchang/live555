#-------------------------------------------------
#
# Project created by QtCreator 2014-08-27T15:14:48
#
#-------------------------------------------------

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt


INCLUDEPATH +=../liveMedia/include ../BasicUsageEnvironment/include \
              ../groupsock/include ../UsageEnvironment/include

INCLUDEPATH  += ../third_party/cppzmq/include

LIBS += -L$$PWD/../libs

win32 {

DEFINES += ZMQ_HAVE_WINDOWS ZMQ_STATIC  __WINDOWS__
LIBS +=  -lliveMedia -lgroupsock -lBasicUsageEnvironment  -lUsageEnvironment
LIBS += -lcppzmq
LIBS += -lws2_32  -lAdvapi32

}
linux {
LIBS +=  -lliveMedia -lgroupsock -lBasicUsageEnvironment  -lUsageEnvironment
LIBS += -lcppzmq -pthread

}

target.path = $$PWD/../bin
INSTALLS += target


SOURCES += \
    live555MediaServer.cpp \
    DynamicRTSPServer.cpp \
    zmqframedsource.cpp \
    QueueServerMediaSubsession.cpp

HEADERS += \
    version.hh \
    DynamicRTSPServer.hh \
    zmqframedsource.h \
    QueueServerMediaSubsession.h
