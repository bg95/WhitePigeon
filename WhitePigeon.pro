#-------------------------------------------------
#
# Project created by QtCreator 2013-07-15T09:38:49
#
#-------------------------------------------------

QT       += core gui opengl multimedia

#needs
#libpulse
#qmultimedia5

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WhitePigeon
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
        myglwidget.cpp \
    core/wpwave.cpp

HEADERS  += mainwindow.h\
            myglwidget.h \
    core/wpwave.h

FORMS    += mainwindow.ui
