



win32 {

DEFINES += _UNICODE UNICODE WIN32_LEAN_AND_MEAN
    Debug {
        DEFINES += _DEBUG
        output_dir = $$PWD/../out/debug
    }

    Release {
        output_dir = $$PWD/../out/release
    }

}

macx {
        output_dir = $$PWD/../out/mac
}

ios {
        DEFINES += POSIX IOS
        output_dir = $$PWD/../out/ios
}

linux {
    DEFINES += POSIX LINUX
}

target.path = $$PWD/../../lib
INSTALLS += target
