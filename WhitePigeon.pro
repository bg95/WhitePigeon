#-------------------------------------------------
#
# Project created by QtCreator 2013-07-15T09:38:49
#
#-------------------------------------------------

QT       += core gui opengl multimedia webkit webkitwidgets network
CONFIG += opengl

#to compile a plug-in
#g++ WPTuningFork.cpp -o  libWPTuningFork.so -fPIC -shared -I /usr/include/qt5/QtCore -I /usr/include/qt5/QtMultimedia -I /usr/include/qt5 -I ./.. -g

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WhitePigeon
TEMPLATE = app
LIBS += -ldl


SOURCES += main.cpp\
    OscilloscopeWindow.cpp \
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
    WPScore/WPAllocator.cpp \
    core/WPOscilloscope.cpp \
    core/WPSynthesizer.cpp \
    core/WPWave.cpp \
    core/WPPipe.cpp \
    core/WPMixer.cpp \
    core/WPSynthesisController.cpp \
    core/WPModifier.cpp \
    core/WPPropertyAndModifiers.cpp \
#    core/WPTuningFork.cpp \
#    core/WPStaccatissimo.cpp \
    WPDLLManager/WPDLLManager.cpp \
    WPDLLManager/WPDLLTimbre.cpp \
    WPDLLManager/WPCallbackManager.cpp \
    WPDLLManager/WPDLLModifier.cpp \
    WPDLLManager/WPDLLTimbreManager.cpp \
    plugins/WPDefaultNoteModifier.cpp \
    plugins/WP12EqualTuning.cpp \
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
    musicshower/getmusicinfodialog.cpp \
    musicshower/musicview.cpp \
    UI/QRecentWebsitesMenu.cpp \
    UI/versiondialog.cpp \
    WPImage/Parallel_Lines.cpp \
    WPImage/Image_Processing.cpp \
    WPImage/Image.cpp

HEADERS  += \
            myglwidget.h \
    OscilloscopeWindow.h \
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
    core/WPTimbre.h \
#    core/WPTuningFork.h \
    core/WPVarTimbre.h \
    core/WPPipe.h \
    core/WPMixer.h \
    core/WPSynthesisController.h \
    core/WPPropertyAndModifiers.h \
    core/WPModifier.h \
#    core/WPStaccatissimo.h \
    WPScore/WPPersistentTree.h \
    WPScore/WPAllocator.h \
    WPDLLManager/WPDLLManager.h \
    WPDLLManager/WPDLLTimbre.h \
    WPDLLManager/WPCallbackManager.h \
    WPDLLManager/WPDLLModifier.h \
    WPDLLManager/WPDLLTimbreManager.h \
    plugins/WPDefaultNoteModifier.h \
    plugins/WP12EqualTuning.h \
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
    musicshower/getmusicinfodialog.h \
    musicshower/musicview.h \
    OscilloscopeWindow.h \
    myglwidget.h \
    UI/QRecentWebsitesMenu.h \
    UI/versiondialog.h \
    WPImage/Parallel_Lines.h \
    WPImage/Image_Processing.h \
    WPImage/Image.h

TRANSLATIONS += Chinese.ts

RESOURCES += \
    whitepigeon.qrc

FORMS += \
    UI/versiondialog.ui
