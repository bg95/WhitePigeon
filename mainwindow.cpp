#include <QtWidgets>

#include "QRecentFilesMenu.h"
#include "OscilloscopeWindow.h"
#include "WPWindow.h"

#include "mainwindow.h"

MainWindow::MainWindow()
{
    /* Private widget settings */
    oscilloscopeWindow = 0;
    countNumber = 0;

    mdiArea = new QMdiArea;
    mdiArea->setParent(this);
    mdiArea->setViewMode(QMdiArea::TabbedView);
    mdiArea->setActivationOrder(QMdiArea::CreationOrder);

    mdiArea->addSubWindow(new WPWindow(this));
    mdiArea->addSubWindow(new WPWindow(this));
    mdiArea->addSubWindow(new WPWindow(this));
    mdiArea->addSubWindow(new WPWindow(this));

    /* MainWindow settings */
    setWindowTitle(tr("WhitePigeon"));
    // setWindowIcon(QIcon(":/images/WhitePigeon.jpg"));
    setContextMenuPolicy(Qt::ActionsContextMenu);
    setCentralWidget(mdiArea);

    /* UI settings */
    createActions();
    createToolBar();
    createMenus();
    createStatusBar();
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
    toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(oscilloscopeAction);
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
