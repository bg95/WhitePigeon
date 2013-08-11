#include <QtWidgets>

#include "OscilloscopeWindow.h"
#include "mainwindow.h"

MainWindow::MainWindow()
{
    oscilloscopeWindow = new OscilloscopeWindow(this);
    setWindowTitle(tr("WhitePigeon"));
    createActions();
    createToolBar();
    createMenus();
    createStatusBar();

    // setWindowIcon(QIcon(":/images/WhitePigeon.jpg"));
}

void MainWindow::createActions()
{
    oscilloscopeAction = new QAction(tr("&Oscilloscope"), this);
    // oscilloscopeAction->setIcon(QIcon(":/images/oscilloscope.jpg"));
    // oscilloscopeAction->setShortcut();
    oscilloscopeAction->setStatusTip("Show an oscilloscope");
    oscilloscopeAction->setToolTip("Show an oscilloscope");
    //oscilloscopeAction->setCheckable(true);
    connect(oscilloscopeAction, SIGNAL(triggered()), this, SLOT(showOscilloscope()));
}

void MainWindow::createMenus()
{
    toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(oscilloscopeAction);
}

void MainWindow::showOscilloscope()
{
    oscilloscopeWindow->show();
}


void MainWindow::createToolBar()
{
    toolBar = addToolBar(tr("&Tools"));
    toolBar->addAction(oscilloscopeAction);
}

void MainWindow::createStatusBar()
{
    QLabel *statusmsg = new QLabel;
    statusBar()->addWidget(statusmsg);
}
