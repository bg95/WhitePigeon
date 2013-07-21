#-------------------------------------------------
#
# Project created by QtCreator 2013-07-15T09:38:49
#
#-------------------------------------------------

QT       += core gui opengl multimedia

#needs
#libpulse-dev
#qtmultimedia5-dev

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WhitePigeon
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
        myglwidget.cpp \
    core/wpwave.cpp \
    core/wpsynthesizer.cpp \
    WPScore/WPScore.cpp \
    WPScore/WPProperty.cpp \
    WPScore/WPPosition.cpp \
    WPScore/WPPart.cpp \
    WPScore/WPNote.cpp \
    WPScore/WPMultinote.cpp \
    WPScore/WPLib.cpp \
    WPScore/WPInterval.cpp

HEADERS  += mainwindow.h\
            myglwidget.h \
    core/wpwave.h \
    core/wpsynthesizer.h \
    WPScore/WPSPosition.h \
    WPScore/WPScore.h \
    WPScore/WPProperty.h \
    WPScore/WPPosition.h \
    WPScore/WPPart.h \
    WPScore/WPNote.h \
    WPScore/WPMultinote.h \
    WPScore/WPLib.h \
    WPScore/WPInterval.h

FORMS    += mainwindow.ui
