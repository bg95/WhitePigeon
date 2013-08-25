#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QAction;
class QMenu;
class OscilloscopeWindow;
class musicScene;
class QGraphicsView;
class QToolBar;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void showOscilloscope();
    //void getMusicInformation();

private:
    void createActions();
    void createMenus();
    void createToolBar();
    void createStatusBar();
    //void drawMusic();
    void readSettings();
    void writeSettings();
    //void showMusic();

    QAction *oscilloscopeAction;
    QAction *newAction;

    QMenu *toolsMenu;
    QMenu *fileMenu;
    QToolBar *toolBar;

    OscilloscopeWindow *oscilloscopeWindow;
    musicScene *scene;
    QGraphicsView *view;
};

#endif
