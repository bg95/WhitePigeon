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

    openAction = new QAction(this);
    openAction->setText(tr("&Open"));
    // openAction->setIcon(QIcon(":/images/open.jpg"));
    openAction->setShortcut(QKeySequence::Open);
    openAction->setStatusTip(tr("Open a file"));
    openAction->setToolTip(tr("Open a file"));
    connect(openAction, SIGNAL(triggered()),
            this, SLOT(loadFile()));

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
    fileMenu->addAction(openAction);

    recentFilesMenu = new QRecentFilesMenu(fileMenu);
    recentFilesMenu->setTitle(tr("&Recent Files"));
    recentFilesMenu->setStatusTip(tr("Recent opening files"));
    connect(recentFilesMenu, SIGNAL(recentFileTriggered(const QString &)),
            this, SLOT(loadFile(const QString &)));

    fileMenu->addMenu(recentFilesMenu);

    toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(oscilloscopeAction);
}

void MainWindow::createToolBar()
{
    /* Create all toolbars */
    fileToolBar = new QToolBar;
    fileToolBar->addAction(newAction);
    fileToolBar->addAction(openAction);
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
    WPWindow *window = createNewChild();
    window->setWindowTitle(tr("untitled %1").arg(countNumber));
    window->show();
}

void MainWindow::loadFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                               tr("Open a file"), ".",
                               tr("Score files (*.01)"));
    if (!fileName.isEmpty())
    {
        loadFile(fileName);
    }
}

void MainWindow::loadFile(const QString& file)
{
    WPWindow *window = findChild(file);
    if (window)
    {
        mdiArea->setActiveSubWindow(window);
        statusBar()->showMessage(tr("File already opened"), 2000);
    }
    else
    {
        window = createNewChild();
        if (window->loadFile(file))
        {
            window->show();
            mdiArea->setActiveSubWindow(window);
            recentFilesMenu->addRecentFile(file);
            statusBar()->showMessage(tr("File loaded"), 2000);
        }
        else
        {
            window->close();
            statusBar()->showMessage(tr("Loading canceled"), 2000);
        }
    }
}

void MainWindow::showOscilloscope()
{
    if (!oscilloscopeWindow)
    {
        oscilloscopeWindow = new OscilloscopeWindow(this);
    }
    oscilloscopeWindow->show();
}

void MainWindow::updateStatusBar(QMdiSubWindow* window)
{
    if (window)
    {
        statusBar()->showMessage(window->windowTitle());
    }
}


/* private functions */

WPWindow* MainWindow::createNewChild()
{
    WPWindow *window = new WPWindow;
    mdiArea->addSubWindow(window);
    return window;
}

WPWindow* MainWindow::findChild(const QString &file)
{
    QString canonical_file_path = QFileInfo(file).canonicalFilePath();
    foreach (QMdiSubWindow *window, mdiArea->subWindowList())
    {
        WPWindow *my_window = dynamic_cast<WPWindow *>(window);
        if (my_window->currentFilePath() == canonical_file_path)
        {
            return my_window;
        }
    }
    return 0;
}
