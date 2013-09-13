#-------------------------------------------------
#
# Project created by QtCreator 2013-07-15T09:38:49
#
#-------------------------------------------------

QT       += core gui opengl multimedia webkit webkitwidgets network

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
    core/WPPipe.cpp \
    core/WPMixer.cpp \
    WPScore/WPPersistentTree.cpp \
#    WPScore/WPAllocator.cpp
    core/WPSynthesisController.cpp \
    UI/WPWindow.cpp \
    UI/QRecentFilesMenu.cpp \
    UI/mainwindow.cpp \
    musicshower/musicwholeitem.cpp \
    musicshower/musictextitem.cpp \
    musicshower/musicscene.cpp \
    musicshower/musicrowitem.cpp \
    musicshower/musiclineitem.cpp \
    musicshower/musicdotitem.cpp \
    musicshower/musicbracketitem.cpp \
    musicshower/musicbaritem.cpp \
    musicshower/musicarcitem.cpp \
    musicshower/getmusicinfodialog.cpp

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
    core/WPTimbre.h \
    core/WPTuningFork.h \
    core/WPVarTimbre.h \
    core/WPPipe.h \
    core/WPMixer.h \
    WPScore/WPPersistentTree.h \
    WPScore/WPAllocator.h \
    core/WPSynthesisController.h \
    UI/WPWindow.h \
    UI/QRecentFilesMenu.h \
    UI/mainwindow.h \
    musicshower/musicwholeitem.h \
    musicshower/musictextitem.h \
    musicshower/musicscene.h \
    musicshower/musicrowitem.h \
    musicshower/musiclineitem.h \
    musicshower/musicdotitem.h \
    musicshower/musicbracketitem.h \
    musicshower/musicbaritem.h \
    musicshower/musicarcitem.h \
    musicshower/getmusicinfodialog.h

TRANSLATIONS += Chinese.ts

RESOURCES += \
    whitepigeon.qrc

FORMS +=
