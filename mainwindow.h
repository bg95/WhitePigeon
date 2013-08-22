#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

/* Class declearation */
class QLabel;
class OscilloscopeWindow;

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
    void drawMusic();

    /* Action list */
    QAction *oscilloscopeAction;

    /* Menu list */
    QMenu *toolsMenu;

    /* ToolBar list */
    QToolBar *toolBar;

    /* Private widget list */
    QLabel *statusmsg;
    OscilloscopeWindow *oscilloscopeWindow;
};

#endif
