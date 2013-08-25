#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

/* Class declearation */
class OscilloscopeWindow;
class QRecentFilesMenu;
class QMdiArea;
class QMdiSubWindow;
class WPWindow;
class WPSynthesisController;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *);

private slots:
    /* Action functions */
    void newFile();
    void loadFile();
    void loadFile(const QString &file);
    bool saveFile();
    bool saveAsFile();
    void closeFile();
    void closeAllFiles();
    void play();
    void stopAll();
    void updateActionsNeedingSubWindow();
    void showOscilloscope();
    void updateStatusBar(QMdiSubWindow *window);

private:
    void createActions();
    void createMenus();
    void createToolBar();
    void createStatusBar();
    void readSettings();
    void writeSettings();

    WPWindow *createNewChild();
    WPWindow *findChild(const QString &file);

    /* Action list */
    QAction *newAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *closeAction;
    QAction *closeAllAction;
    QAction *exitAction;
    QAction *playAction;
    QAction *stopAction;
    QAction *oscilloscopeAction;

    /* Menu list */
    QMenu *fileMenu;
    QRecentFilesMenu *recentFilesMenu;
    QMenu *musicMenu;
    QMenu *toolsMenu;

    /* ToolBar list */
    QToolBar *fileToolBar;
    QToolBar *musicToolBar;
    QToolBar *toolBar;

    /* Private widget list */
    WPSynthesisController *controller;
    QMdiArea *mdiArea;
    OscilloscopeWindow *oscilloscopeWindow;
    int countNumber;
};

#endif
