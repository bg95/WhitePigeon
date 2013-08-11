#include <QAction>
#include <QMenu>
#include <QMenuBar>

#include "OscilloscopeWindow.h"
#include "mainwindow.h"

MainWindow::MainWindow()
{
    oscilloscopeWindow = new OscilloscopeWindow(this);

    createActions();
    createMenus();

    oscilloscopeWindow->show();
    // setWindowIcon(QIcon(":/images/WhitePigeon.jpg"));
}

void MainWindow::createActions()
{
    oscilloscopeAction = new QAction(tr("Oscilloscope"), this);
    // oscilloscopeAction->setIcon(QIcon(":/images/oscilloscope.jpg"));
    // oscilloscopeAction->setShortcut();
    // oscilloscopeAction->setStatusTip("Show an oscilloscope");
    connect(oscilloscopeAction, SIGNAL(triggered()), this,
            SLOT(showOscilloscope()));
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
