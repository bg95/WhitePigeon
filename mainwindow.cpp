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
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
            this, SLOT(updateStatusBar(QMdiSubWindow*)));

    /* MainWindow settings */
    setWindowTitle(tr("WhitePigeon"));
    // setWindowIcon(QIcon(":/images/WhitePigeon.jpg"));
    setContextMenuPolicy(Qt::ActionsContextMenu);
    setCentralWidget(mdiArea);

    /* UI settings */
    createActions();
    createToolBar();
    createMenus();
}

MainWindow::~MainWindow()
{
    delete oscilloscopeWindow;
}

void MainWindow::createActions()
{
    newAction = new QAction(this);
    newAction->setText(tr("&New"));
    // newAction->setIcon(QIcon(":/images/new.jpg"));
    newAction->setShortcut(QKeySequence::New);
    newAction->setStatusTip(tr("Create a new file"));
    newAction->setToolTip(tr("Create a new file"));
    connect(newAction, SIGNAL(triggered()),
            this, SLOT(newFile()));

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
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);

    toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(oscilloscopeAction);
}

void MainWindow::createToolBar()
{
    /* Create all toolbars */
    fileToolBar = new QToolBar;
    fileToolBar->addAction(newAction);
    fileToolBar->setToolTip(tr("&File"));

    toolBar = new QToolBar;
    toolBar->addAction(oscilloscopeAction);
    toolBar->setToolTip(tr("&Tools"));

    /* add the toolbars to the MainWindow */
    addToolBar(fileToolBar);
    addToolBar(toolBar);
}


/* private slots */

void MainWindow::newFile()
{
    ++countNumber;
    WPWindow *window = new WPWindow;
    window->setWindowTitle(tr("untitled %1").arg(countNumber));
    mdiArea->addSubWindow(window);
    window->show();
}

void MainWindow::showOscilloscope()
{
    if (!oscilloscopeWindow)
        oscilloscopeWindow = new OscilloscopeWindow(this);
    oscilloscopeWindow->show();
}

void MainWindow::updateStatusBar(QMdiSubWindow* window)
{
    if (window)
        statusBar()->showMessage(window->windowTitle());
}
