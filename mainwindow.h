#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

/* Class declearation */
class QLabel;
class OscilloscopeWindow;
class QRecentFilesMenu;
class QMdiArea;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

private slots:
    /* Action functions */
    void showOscilloscope();

private:
    void createActions();
    void createMenus();
    void createToolBar();
    void createStatusBar();

    /* Action list */
    QAction *oscilloscopeAction;

    /* Menu list */
    QRecentFilesMenu *recentFilesMenu;
    QMenu *toolsMenu;

    /* ToolBar list */
    QToolBar *toolBar;

    /* Private widget list */
    QMdiArea *mdiArea;
    QLabel *statusmsg;
    OscilloscopeWindow *oscilloscopeWindow;
    int countNumber;
};

#endif
