QT += quick multimedia

HEADERS += \
    $$PWD/qrdecodeitem.h \
    $$PWD/qrdecodeplugin.h

SOURCES += \
    $$PWD/qrdecodeitem.cpp \
    $$PWD/qrdecodeplugin.cpp

unix {
    android {
        INCLUDEPATH += $$PWD/../ext/android_$$ANDROID_TARGET_ARCH/include
        LIBS += -L$PWD/../ext/android_$$ANDROID_TARGET_ARCH/lib -liconv -lzbarjni
    } else {
        INCLUDEPATH += $$PWD/../ext/gnu-linux-amd64/include
        LIBS += -L$$PWD/../ext/gnu-linux-amd64/lib -lzbar
    }
}
