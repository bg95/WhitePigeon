#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

/* Class declearation */
class QLabel;
class OscilloscopeWindow;
class QRecentFilesMenu;
class QMdiArea;
class QMdiSubWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

private slots:
    /* Action functions */
    void newFile();
    void showOscilloscope();
    void updateStatusBar(QMdiSubWindow* window);

private:
    void createActions();
    void createMenus();
    void createToolBar();
    void createStatusBar();

    /* Action list */
    QAction *newAction;
    QAction *oscilloscopeAction;

    /* Menu list */
    QMenu *fileMenu;
    QRecentFilesMenu *recentFilesMenu;
    QMenu *toolsMenu;

    /* ToolBar list */
    QToolBar *fileToolBar;
    QToolBar *toolBar;

    /* Private widget list */
    QMdiArea *mdiArea;
    OscilloscopeWindow *oscilloscopeWindow;
    int countNumber;
};

#endif
