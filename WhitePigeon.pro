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
        myglwidget.cpp \
    WPScore/WPScore.cpp \
    WPScore/WPProperty.cpp \
    WPScore/WPPosition.cpp \
    WPScore/WPPart.cpp \
    WPScore/WPNote.cpp \
    WPScore/WPMultinote.cpp \
    WPScore/WPLib.cpp \
    WPScore/WPInterval.cpp \
    core/WPOscilloscope.cpp \
    core/WPSynthesizer.cpp \
    core/WPWave.cpp \
    OscilloscopeWindow.cpp \
    mainwindow.cpp \
    core/WPPipe.cpp \
    core/WPMixer.cpp \
    WPScore/WPPersistentTree.cpp \
#    WPScore/WPAllocator.cpp

HEADERS  +=\
            myglwidget.h \
    WPScore/WPSPosition.h \
    WPScore/WPScore.h \
    WPScore/WPProperty.h \
    WPScore/WPPosition.h \
    WPScore/WPPart.h \
    WPScore/WPNote.h \
    WPScore/WPMultinote.h \
    WPScore/WPLib.h \
    WPScore/WPInterval.h \
    core/WPOscilloscope.h \
    core/WPSynthesizer.h \
    core/WPWave.h \
    OscilloscopeWindow.h \
    mainwindow.h \
    core/WPTimbre.h \
    core/WPTuningFork.h \
    core/WPVarTimbre.h \
    core/WPPipe.h \
    core/WPMixer.h \
    WPScore/WPPersistentTree.h \
    WPScore/WPAllocator.h
