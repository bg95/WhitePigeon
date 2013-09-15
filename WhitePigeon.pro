#-------------------------------------------------
#
# Project created by QtCreator 2013-07-15T09:38:49
#
#-------------------------------------------------

QT       += core gui opengl multimedia webkit webkitwidgets network
CONFIG += opengl

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
    WPScore/WPPersistentTree.cpp \
#    core/WPSynthesizer.cpp \
#    core/WPWave.cpp \
#    core/WPPipe.cpp \
#    core/WPMixer.cpp \
#    core/WPSynthesisController.cpp \
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
#    musicshower/getmusicinfodialog.cpp \
    musicshower/musicview.cpp \
    musicshower/musicrepeatitem.cpp \
#    core/WPPropertyAndModifiers.cpp \
#    core/WPOscilloscope.cpp \
#    core/WPModifier.cpp \
#    WPDLLManager/WPDLLTimbreManager.cpp \
#    WPDLLManager/WPDLLTimbre.cpp \
#    WPDLLManager/WPDLLModifier.cpp \
#    WPDLLManager/WPDLLManager.cpp \
#    WPDLLManager/WPCallbackManager.cpp \
#    plugins/WPDefaultNoteModifier.cpp \
#    plugins/WP12EqualTuning.cpp \
#    OscilloscopeWindow.cpp \
    myglwidget.cpp \
    UI/QRecentWebsitesMenu.cpp \
    UI/versiondialog.cpp \
#    plugins/WPTempoModifier.cpp \
#    plugins/WPAmpModifier.cpp \
    WPImage/Parallel_Lines.cpp \
    WPImage/Image_Processing.cpp \
    WPImage/Image.cpp

HEADERS  +=\
    WPScore/WPSPosition.h \
    core/WPOscilloscope.cpp \
    core/WPSynthesizer.cpp \
    core/WPWave.cpp \
    OscilloscopeWindow.cpp \
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
    WPWindow.cpp

HEADERS  +=\
            myglwidget.h \
    WPScore/WPScore.h \
    WPScore/WPProperty.h \
    WPScore/WPPosition.h \
    WPScore/WPPart.h \
    WPScore/WPNote.h \
    WPScore/WPMultinote.h \
    WPScore/WPLib.h \
    WPScore/WPInterval.h \
#    core/WPSynthesizer.h \
#    core/WPWave.h \
#    core/WPTimbre.h \
#    core/WPVarTimbre.h \
#    core/WPPipe.h \
#    core/WPMixer.h \
    WPScore/WPPersistentTree.h \
    WPScore/WPAllocator.h \
#    core/WPSynthesisController.h \
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
    musicshower/musicrepeatitem.h \
    musicshower/getmusicinfodialog.h \
    musicshower/musicview.h \
#    core/WPPropertyAndModifiers.h \
#    core/WPOscilloscope.h \
#    core/WPModifier.h \
#    WPDLLManager/WPDLLTimbreManager.h \
#    WPDLLManager/WPDLLTimbre.h \
#    WPDLLManager/WPDLLModifier.h \
#    WPDLLManager/WPDLLManager.h \
#    WPDLLManager/WPCallbackManager.h \
#    plugins/WPDefaultNoteModifier.h \
#    plugins/WP12EqualTuning.h \
#    OscilloscopeWindow.h \
    myglwidget.h \
    UI/QRecentWebsitesMenu.h \
    UI/versiondialog.h \
#    plugins/WPTempoModifier.h \
#    plugins/WPAmpModifier.h \
    WPImage/Parallel_Lines.h \
    WPImage/Image_Processing.h \
    WPImage/Image.h

TRANSLATIONS += Chinese.ts

RESOURCES += \
    whitepigeon.qrc

FORMS += \
    UI/versiondialog.ui
