TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt



INCLUDEPATH +=../liveMedia/include ../BasicUsageEnvironment/include \
              ../groupsock/include ../UsageEnvironment/include


LIBS += -L$$PWD/../lib
LIBS +=  -lliveMedia -lgroupsock -lBasicUsageEnvironment  -lUsageEnvironment
#    LIBS += -lcppzmq -ljsoncpp
#    LIBS += -pthread -ldl

target.path = $$PWD/../lib
INSTALLS += target


SOURCES += \
    live555MediaServer.cpp \
    DynamicRTSPServer.cpp

HEADERS += \
    version.hh \
    DynamicRTSPServer.hh

