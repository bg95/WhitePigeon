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
    core/WPOscilloscope.cpp \
    core/WPSynthesizer.cpp \
    core/WPWave.cpp \
    OscilloscopeWindow.cpp \
    mainwindow.cpp \
    core/WPPipe.cpp \
	core/WPMixer.cpp \
    core/WPSynthesisController.cpp \
    core/WPTuningFork.cpp \
    QRecentFilesMenu.cpp \
    WPWindow.cpp

HEADERS  +=\
			myglwidget.h \
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
    core/WPSynthesisController.h \
    QRecentFilesMenu.h \
    WPWindow.h

TRANSLATIONS += Chinese.ts

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/release/ -lWPScore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/debug/ -lWPScore
else:unix: LIBS += -L$$PWD/lib/ -lWPScore

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include
