QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
greaterThan(QT_MAJOR_VERSION, 4) {
    TARGET_ARCH=$${QT_ARCH}
} else {
    TARGET_ARCH=$${QMAKE_HOST.arch}
}

contains(TARGET_ARCH, x86_64) {
    ARCHITECTURE = x64
} else {
    ARCHITECTURE = x86
}

win32-g++:contains(ARCHITECTURE, x86): {
    INCLUDEPATH += C:/boost/boost_mingw1120_64/include/boost-1_71
}
win32-g++:contains(ARCHITECTURE, x64): {
    INCLUDEPATH += C:/boost/boost_mingw1120_64/include/boost-1_71
}

win32-g++ {
    INCLUDEPATH += C:/boost/boost_mingw1120_64/include/boost-1_71
    LIBS += "-LC:/boost/boost_mingw1120_64/lib" \
#                -llibboost_date_time-mgw11-mt-d-x64-1_71
}


SOURCES += \
    main.cpp \
    widget.cpp

HEADERS += \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qrc.qrc
