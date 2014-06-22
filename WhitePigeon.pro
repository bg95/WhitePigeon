#-------------------------------------------------
#
# Project created by QtCreator 2013-07-15T09:38:49
#
#-------------------------------------------------

QT       += core gui opengl multimedia webkit webkitwidgets network
CONFIG += opengl
LIBS += -ldl
QMAKE_CXXFLAGS += -rdynamic

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WhitePigeon
TEMPLATE = app


SOURCES += main.cpp\
    core/WPSynthesizer.cpp \
#    core/WPWave.cpp \
    core/WPPipe.cpp \
    core/WPMixer.cpp \
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
    musicshower/musicview.cpp \
    musicshower/musicrepeatitem.cpp \
    core/WPPropertyAndModifiers.cpp \
    core/WPOscilloscope.cpp \
    WPDLLManager/WPDLLTimbreManager.cpp \
    WPDLLManager/WPDLLManager.cpp \
    WPDLLManager/WPCallbackManager.cpp \
    plugins/WPDefaultNoteModifier.cpp \
    plugins/WP12EqualTuning.cpp \
    OscilloscopeWindow.cpp \
    myglwidget.cpp \
    UI/QRecentWebsitesMenu.cpp \
    UI/versiondialog.cpp \
    WPImage/Parallel_Lines.cpp \
    WPImage/Image_Processing.cpp \
    WPImage/Image.cpp \
    core/WPTimbre.cpp \
#    core/WPModifierInternal.cpp \
#    core/WPModifier.cpp

HEADERS  +=\
    core/WPSynthesizer.h \
    include/core/WPWave.h \
    include/core/WPTimbre.h \
    core/WPPipe.h \
    core/WPMixer.h \
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
    musicshower/musicrepeatitem.h \
    musicshower/getmusicinfodialog.h \
    musicshower/musicview.h \
    core/WPPropertyAndModifiers.h \
    core/WPOscilloscope.h \
    WPDLLManager/WPDLLTimbreManager.h \
    WPDLLManager/WPDLLManager.h \
    WPDLLManager/WPCallbackManager.h \
    plugins/WPDefaultNoteModifier.h \
    plugins/WP12EqualTuning.h \
    OscilloscopeWindow.h \
    myglwidget.h \
    UI/QRecentWebsitesMenu.h \
    UI/versiondialog.h \
    WPImage/Parallel_Lines.h \
    WPImage/Image_Processing.h \
    WPImage/Image.h \
#    core/WPModifierInternal.h \
    include/core/WPModifier.h \
    include/WPScore/WPScore.h \
    include/WPScore/WPProperty.h \
    include/WPScore/WPPosition.h \
    include/WPScore/WPPersistentTree.h \
    include/WPScore/WPPart.h \
    include/WPScore/WPNote.h \
    include/WPScore/WPMultinote.h \
    include/WPScore/WPLib.h \
    include/WPScore/WPInterval.h \
    include/WPScore/WPAllocator.h

TRANSLATIONS += Chinese.ts

RESOURCES += \
    whitepigeon.qrc

FORMS += \
    UI/versiondialog.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/release/ -lWPScore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/debug/ -lWPScore
else:unix: LIBS += -L$$PWD/lib/ -lWPScore

INCLUDEPATH += $$PWD/include/
DEPENDPATH += $$PWD/include/

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/release/ -lWPPlugin
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/debug/ -lWPPlugin
else:unix: LIBS += -L$$PWD/lib/ -lWPPlugin

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include
