#-------------------------------------------------
#
# Project created by QtCreator 2017-09-08T09:17:39
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fos
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    foslogdialog.cpp \
    fosmainwindow.cpp \
    foschatwidget.cpp \
    fosuseritems.cpp \
    fostcpsocket.cpp \
    fosloginctrl.cpp \
    fosregisterctrl.cpp \
    fosmsgitems.cpp \
    fosdatabase.cpp \
    fosfileclient.cpp \
    fosfileserver.cpp

HEADERS += \
    foslogdialog.h \
    fosmainwindow.h \
    foschatwidget.h \
    fosuseritems.h \
    fostcpsocket.h \
    fosloginctrl.h \
    fosconstant.h \
    fosregisterctrl.h \
    fosmsgitems.h \
    fosdatabase.h \
    fosfileclient.h \
    fosfileserver.h

FORMS += \
    foslogdialog.ui

RESOURCES += \
    images.qrc \
    qss.qrc \
    text.qrc

DISTFILES += \
    icon.rc

RC_FILE = icon.rc
