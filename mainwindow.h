#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QAction;
class QMenu;
class OscilloscopeWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private slots:
    void showOscilloscope();

private:
    void createActions();
    void createMenus();

    QAction *oscilloscopeAction;

    QMenu *toolsMenu;

    OscilloscopeWindow *oscilloscopeWindow;
};

#endif
