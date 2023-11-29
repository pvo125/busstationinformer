QT       += core gui network serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    http.cpp \
    main.cpp \
    mainwindow.cpp \
    modem.cpp \
    wiring.cpp

HEADERS += \
    http.h \
    mainwindow.h \
    modem.h \
    wiring.h

unix:   LIBS += -lwiringPi


#win32:  LIBS+=-LC:/OpenSSL-Win32/lib/MinGW -llibeay32 -lssleay32

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /home/pi/app
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    gsmSignal.qrc

DISTFILES +=
