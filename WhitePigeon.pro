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
    WPScore/WPScore.cpp \
    WPScore/WPProperty.cpp \
    WPScore/WPPosition.cpp \
    WPScore/WPPart.cpp \
    WPScore/WPNote.cpp \
    WPScore/WPMultinote.cpp \
    WPScore/WPLib.cpp \
    WPScore/WPInterval.cpp \
    core/WPSynthesizer.cpp \
    core/WPWave.cpp \
    mainwindow.cpp \
    core/WPPipe.cpp \
    core/WPMixer.cpp \
    WPScore/WPPersistentTree.cpp \
    musictextitem.cpp \
    musicscene.cpp \
    musicarcitem.cpp \
    getmusicinfodialog.cpp \
    musicbracketitem.cpp \
    musicdotitem.cpp \
    musiclineitem.cpp \
    musicbaritem.cpp \
    musicrowitem.cpp \
    musicwholeitem.cpp \
    core/WPSynthesisController.cpp \
    QRecentFilesMenu.cpp \
    WPWindow.cpp \
    addressbar.cpp \
    musicwholeitem.cpp \
    musictextitem.cpp \
    musicscene.cpp \
    musicrowitem.cpp \
    musiclineitem.cpp \
    musicdotitem.cpp \
    musicbracketitem.cpp \
    musicbaritem.cpp \
    musicarcitem.cpp \
    getmusicinfodialog.cpp

HEADERS  +=\
    WPScore/WPSPosition.h \
    WPScore/WPScore.h \
    WPScore/WPProperty.h \
    WPScore/WPPosition.h \
    WPScore/WPPart.h \
    WPScore/WPNote.h \
    WPScore/WPMultinote.h \
    WPScore/WPLib.h \
    WPScore/WPInterval.h \
    core/WPSynthesizer.h \
    core/WPWave.h \
    mainwindow.h \
    core/WPTimbre.h \
    core/WPTuningFork.h \
    core/WPVarTimbre.h \
    core/WPPipe.h \
    core/WPMixer.h \
    WPScore/WPPersistentTree.h \
    WPScore/WPAllocator.h \
    core/WPSynthesisController.h \
    musictextitem.h \
    musicscene.h \
    musicarcitem.h \
    getmusicinfodialog.h \
    musicbracketitem.h \
    musicdotitem.h \
    musiclineitem.h \
    musicbaritem.h \
    musicrowitem.h \
    musicwholeitem.h \
    QRecentFilesMenu.h \
    WPWindow.h \
    addressbar.h \
    musicwholeitem.h \
    musictextitem.h \
    musicscene.h \
    musicrowitem.h \
    musiclineitem.h \
    musicdotitem.h \
    musicbracketitem.h \
    musicbaritem.h \
    musicarcitem.h \
    getmusicinfodialog.h

TRANSLATIONS += Chinese.ts
