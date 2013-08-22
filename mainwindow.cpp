#include <QtWidgets>

#include "OscilloscopeWindow.h"
#include "mainwindow.h"

MainWindow::MainWindow()
{
    /* MainWindow settings */
    setWindowTitle(tr("WhitePigeon"));
    // setWindowIcon(QIcon(":/images/WhitePigeon.jpg"));
    setContextMenuPolicy(Qt::ActionsContextMenu);

    /* Private widget settings */
    oscilloscopeWindow = 0;

    /* UI settings */
    createActions();
    createToolBar();
    createMenus();
    createStatusBar();
    drawMusic();
}

MainWindow::~MainWindow()
{
    delete oscilloscopeWindow;
}

void MainWindow::createActions()
{
    oscilloscopeAction = new QAction(this);
    oscilloscopeAction->setText(tr("&Oscilloscope"));
    // oscilloscopeAction->setIcon(QIcon(":/images/oscilloscope.jpg"));
    // oscilloscopeAction->setShortcut();
    oscilloscopeAction->setStatusTip(tr("Show an oscilloscope"));
    oscilloscopeAction->setToolTip(tr("Show an oscilloscope"));
    connect(oscilloscopeAction, SIGNAL(triggered()),
            this, SLOT(showOscilloscope()));
}

void MainWindow::createMenus()
{
    /* Create all menus */
    toolsMenu = new QMenu;
    toolsMenu->setTitle(tr("&Tools"));
    toolsMenu->addAction(oscilloscopeAction);

    /* add the menus to the MainWindow */
    menuBar()->addMenu(toolsMenu);
}

void MainWindow::showOscilloscope()
{
    if (!oscilloscopeWindow)
        oscilloscopeWindow = new OscilloscopeWindow(this);
    oscilloscopeWindow->show();
}

void MainWindow::createToolBar()
{
    /* Create all toolbars */
    toolBar = new QToolBar;
    toolBar->addAction(oscilloscopeAction);
    toolBar->setToolTip(tr("&Tools"));

    /* add the toolbars to the MainWindow */
    addToolBar(toolBar);
}

void MainWindow::createStatusBar()
{
    statusmsg = new QLabel;
    statusBar()->addWidget(statusmsg);
}

void MainWindow::drawMusic()
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    QGraphicsView *view = new QGraphicsView(scene, this);
    setCentralWidget(view);
}
