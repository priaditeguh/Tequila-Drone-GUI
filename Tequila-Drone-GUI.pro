#-------------------------------------------------
#
# Project created by QtCreator 2017-10-03T19:39:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Tequila-Drone-GUI
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
        mainwindow.cpp \
        qcustomplot.cpp \
    qcgaugewidget.cpp \
    hiredis/async.c \
    hiredis/dict.c \
    hiredis/hiredis.c \
    hiredis/net.c \
    hiredis/read.c \
    hiredis/sds.c

HEADERS += \
        mainwindow.h \
        qcustomplot.h \
    qcgaugewidget.h \
    hiredis/qt.h \
    hiredis/async.h \
    hiredis/dict.h \
    hiredis/fmacros.h \
    hiredis/hiredis.h \
    hiredis/net.h \
    hiredis/read.h \
    hiredis/sds.h \
    hiredis/sdsalloc.h \
    hiredis/win32.h \
    hiredis/async.h \
    hiredis/dict.h \
    hiredis/fmacros.h \
    hiredis/hiredis.h \
    hiredis/net.h

FORMS += \
        mainwindow.ui
