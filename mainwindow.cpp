#include <QtWidgets>

#include "QRecentFilesMenu.h"
#include "core/WPSynthesisController.h"
#include "OscilloscopeWindow.h"
#include "WPWindow.h"

#include "mainwindow.h"

MainWindow::MainWindow()
{
    /* Private widget settings */
    controller = new WPSynthesisController(this);
    oscilloscopeWindow = 0;
    countNumber = 0;

    mdiArea = new QMdiArea;
    mdiArea->setParent(this);
    mdiArea->setViewMode(QMdiArea::TabbedView);
    mdiArea->setActivationOrder(QMdiArea::CreationOrder);
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
            this, SLOT(updateStatusBar(QMdiSubWindow*)));
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
            this, SLOT(updateActionsNeedingSubWindow()));

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
    openAction->setText(tr("&Open..."));
    // openAction->setIcon(QIcon(":/images/open.jpg"));
    openAction->setShortcut(QKeySequence::Open);
    openAction->setStatusTip(tr("Open an existing file"));
    openAction->setToolTip(tr("Open an existing file"));
    connect(openAction, SIGNAL(triggered()),
            this, SLOT(loadFile()));

    saveAction = new QAction(this);
    saveAction->setText(tr("&Save"));
    // saveAction->setIcon(QIcon(":/images/save.jpg"));
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setStatusTip(tr("Save the file to disk"));
    saveAction->setToolTip(tr("Save the file"));
    saveAction->setEnabled(false);
    connect(saveAction, SIGNAL(triggered()),
            this, SLOT(saveFile()));

    saveAsAction = new QAction(this);
    saveAsAction->setText(tr("Save &As..."));
    // saveAsAction->setIcon(QIcon(":/images/saveas.jpg"));
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    saveAsAction->setStatusTip(tr("Save the file under a new name"));
    saveAsAction->setToolTip(tr("Save the file under a new name"));
    saveAsAction->setEnabled(false);
    connect(saveAsAction, SIGNAL(triggered()),
            this, SLOT(saveAsFile()));

    playAction = new QAction(this);
    playAction->setText(tr("Play"));
    // playAction->setIcon(QIcon(":/images/play.jpg"));
    // playAction->setShortcut();
    playAction->setStatusTip(tr("Play the score"));
    playAction->setToolTip(tr("Play the score"));
    playAction->setEnabled(false);
    connect(playAction, SIGNAL(triggered()),
            this, SLOT(play()));

    stopAction = new QAction(this);
    stopAction->setText(tr("Stop"));
    // stopAction->setIcon(QIcon(":/images/stop.jpg"));
    // stopAction->setShortcut();
    stopAction->setStatusTip(tr("Stop playing"));
    stopAction->setToolTip(tr("Stop playing"));
    connect(stopAction, SIGNAL(triggered()),
            this, SLOT(stopAll()));

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
    fileMenu->addSeparator();
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);

    musicMenu = menuBar()->addMenu(tr("&Music"));
    musicMenu->addAction(playAction);
    musicMenu->addAction(stopAction);

    toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(oscilloscopeAction);
}

void MainWindow::createToolBar()
{
    /* Create all toolbars */
    fileToolBar = new QToolBar;
    fileToolBar->addAction(newAction);
    fileToolBar->addAction(openAction);
    fileToolBar->addAction(saveAction);
    fileToolBar->setToolTip(tr("File"));

    musicToolBar = new QToolBar;
    musicToolBar->addAction(playAction);
    musicToolBar->addAction(stopAction);
    musicToolBar->setToolTip(tr("Music"));

    toolBar = new QToolBar;
    toolBar->addAction(oscilloscopeAction);
    toolBar->setToolTip(tr("Tools"));

    /* add the toolbars to the MainWindow */
    addToolBar(fileToolBar);
    addToolBar(musicToolBar);
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

bool MainWindow::saveFile()
{
    WPWindow *window = dynamic_cast<WPWindow *>(mdiArea->activeSubWindow());
    if (window->isSaved())
    {
        if (window->saveFile())
        {
            statusBar()->showMessage(tr("File saved"), 2000);
            return true;
        }
        else
        {
            statusBar()->showMessage(tr("Saving failed"), 2000);
            return false;
        }
    }
    else
    {
        return saveAsFile();
    }
}

bool MainWindow::saveAsFile()
{
    WPWindow *window = dynamic_cast<WPWindow *> (mdiArea->activeSubWindow());
    QString file = QFileDialog::getSaveFileName(this, tr("Save as"));
    if (file.isEmpty())
    {
        return false;
    }
    else
    {
        if (window->saveFile(file))
        {
            statusBar()->showMessage(tr("File saved"), 2000);
            return true;
        }
        else
        {
            statusBar()->showMessage(tr("Saving failed"), 2000);
            return false;
        }
    }
}

void MainWindow::play()
{
    WPWindow *window = dynamic_cast<WPWindow *>(mdiArea->activeSubWindow());
    window->play_with(controller);
}

void MainWindow::stopAll()
{
    // controller->stopAll();
}

void MainWindow::updateActionsNeedingSubWindow()
{
    bool hasActiveWindow = (mdiArea->activeSubWindow() != NULL);

    saveAction->setEnabled(hasActiveWindow);
    saveAsAction->setEnabled(hasActiveWindow);
    playAction->setEnabled(hasActiveWindow);
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
