#include <QtWidgets>

#include "QRecentFilesMenu.h"
#include "core/WPSynthesisController.h"
#include "OscilloscopeWindow.h"
#include "WPWindow.h"
#include "addressbar.h"

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
    mdiArea->setTabsMovable(true);
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
    createMenus();
    createToolBar();
    createAddressBar();
    statusBar();
    createContextMenu();

    readSettings();
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

    closeAction = new QAction(this);
    closeAction->setText(tr("Close"));
    // closeAction->setIcon(QIcon(":/images/close.jpg"));
    closeAction->setShortcut(QKeySequence::Close);
    closeAction->setStatusTip(tr("Close the file"));
    closeAction->setToolTip(tr("Close the file"));
    closeAction->setEnabled(false);
    connect(closeAction, SIGNAL(triggered()),
            this, SLOT(closeFile()));

    closeAllAction = new QAction(this);
    closeAllAction->setText(tr("Close All"));
    // closeAllAction->setIcon(QIcon(":/images/closeall.jpg"));
    // closeAllAction->setShortcut();
    closeAllAction->setStatusTip(tr("Close all files"));
    closeAllAction->setToolTip(tr("Close all files"));
    closeAllAction->setEnabled(false);
    connect(closeAllAction, SIGNAL(triggered()),
            this, SLOT(closeAllFiles()));

    exitAction = new QAction(this);
    exitAction->setText(tr("E&xit"));
    // exitAction->setIcon(QIcon(":/images/exit.jpg"));
    exitAction->setShortcut(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Exit the application"));
    exitAction->setToolTip(tr("Exit the application"));
    connect(exitAction, SIGNAL(triggered()),
            this, SLOT(close()));

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

    fileToolViewAction = new QAction(this);
    fileToolViewAction->setText(tr("File Tool Bar"));
    fileToolViewAction->setCheckable(true);
    fileToolViewAction->setStatusTip(tr("Show file tool bar or not"));
    fileToolViewAction->setToolTip(tr("Show file tool bar or not"));

    musicToolViewAction = new QAction(this);
    musicToolViewAction->setText(tr("Music Tool Bar"));
    musicToolViewAction->setCheckable(true);
    musicToolViewAction->setStatusTip(tr("Show music tool bar or not"));
    musicToolViewAction->setToolTip(tr("Show music tool bar or not"));

    toolToolViewAction = new QAction(this);
    toolToolViewAction->setText(tr("Tool Tool Bar"));
    toolToolViewAction->setCheckable(true);
    toolToolViewAction->setStatusTip(tr("Show tool tool bar or not"));
    toolToolViewAction->setToolTip(tr("Show tool tool bar or not"));

    addressViewAction = new QAction(this);
    addressViewAction->setText(tr("Address Bar"));
    addressViewAction->setCheckable(true);
    addressViewAction->setStatusTip(tr("Show address bar or not"));
    addressViewAction->setToolTip(tr("Show address bar or not"));

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
    connect(recentFilesMenu, SIGNAL(recentFileTriggered(const QString &)),
            this, SLOT(loadFile(const QString &)));

    fileMenu->addMenu(recentFilesMenu);
    fileMenu->addSeparator();
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(closeAction);
    fileMenu->addAction(closeAllAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    musicMenu = menuBar()->addMenu(tr("&Music"));
    musicMenu->addAction(playAction);
    musicMenu->addAction(stopAction);

    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(addressViewAction);

    toolsViewMenu = new QMenu(viewMenu);
    toolsViewMenu->setTitle(tr("Tool Bar"));
    toolsViewMenu->addAction(fileToolViewAction);
    toolsViewMenu->addAction(musicToolViewAction);
    toolsViewMenu->addAction(toolToolViewAction);

    viewMenu->addMenu(toolsViewMenu);

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
    fileToolBar->addSeparator();
    fileToolBar->addAction(closeAction);
    fileToolBar->addAction(closeAllAction);
    fileToolBar->setToolTip(tr("File"));
    connect(fileToolViewAction, SIGNAL(triggered(bool)),
            fileToolBar, SLOT(setVisible(bool)));

    musicToolBar = new QToolBar;
    musicToolBar->addAction(playAction);
    musicToolBar->addAction(stopAction);
    musicToolBar->setToolTip(tr("Music"));
    connect(musicToolViewAction, SIGNAL(triggered(bool)),
            musicToolBar, SLOT(setVisible(bool)));

    toolToolBar = new QToolBar;
    toolToolBar->addAction(oscilloscopeAction);
    toolToolBar->setToolTip(tr("Tools"));
    connect(toolToolViewAction, SIGNAL(triggered(bool)),
            toolToolBar, SLOT(setVisible(bool)));

    /* add the toolbars to the MainWindow */
    addToolBar(fileToolBar);
    addToolBar(musicToolBar);
    addToolBar(toolToolBar);
}

void MainWindow::createAddressBar()
{
    addressBar = new AddressBar(this);
    connect(addressBar, SIGNAL(go(QString)),
            this, SLOT(loadFile(QString)));
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
            addressBar, SLOT(showPath(QMdiSubWindow*)));

    addressDock = new QDockWidget(this);
    addressDock->setAllowedAreas(Qt::TopDockWidgetArea);
    addressDock->setWidget(addressBar);
    addressDock->setFeatures(QDockWidget::DockWidgetVerticalTitleBar);
    connect(addressViewAction, SIGNAL(triggered(bool)),
            addressDock, SLOT(setVisible(bool)));
    addDockWidget(Qt::TopDockWidgetArea, addressDock);
}

void MainWindow::createContextMenu()
{
    addAction(fileToolViewAction);
    addAction(musicToolViewAction);
    addAction(toolToolViewAction);
    addAction(addressViewAction);
}

void MainWindow::readSettings()
{
    QSettings settings("THU3000", "WhitePigeon");

    settings.beginGroup("MainWindow");
    resize(settings.value("size").toSize());
    move(settings.value("pos").toPoint());
    settings.endGroup();

    settings.beginGroup("RecentFiles");
    recentFilesMenu->restoreState(settings.value("files").toByteArray());
    settings.endGroup();

    settings.beginGroup("ToolBar");
    fileToolViewAction->setChecked(settings.value("file").toBool());
    fileToolBar->setVisible(settings.value("file").toBool());
    musicToolViewAction->setChecked(settings.value("music").toBool());
    musicToolBar->setVisible(settings.value("music").toBool());
    toolToolViewAction->setChecked(settings.value("tool").toBool());
    toolToolBar->setVisible(settings.value("tool").toBool());
    settings.endGroup();

    settings.beginGroup("AddressBar");
    addressViewAction->setChecked(settings.value("visible").toBool());
    addressDock->setVisible(settings.value("visible").toBool());
    settings.endGroup();
}

void MainWindow::writeSettings()
{
    QSettings settings("THU3000", "WhitePigeon");

    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.endGroup();

    settings.beginGroup("RecentFiles");
    settings.setValue("files", recentFilesMenu->saveState());
    settings.endGroup();

    settings.beginGroup("ToolBar");
    settings.setValue("file", fileToolBar->isVisible());
    settings.setValue("music", musicToolBar->isVisible());
    settings.setValue("tool", toolToolBar->isVisible());
    settings.endGroup();

    settings.beginGroup("AddressBar");
    settings.setValue("visible", addressDock->isVisible());
    settings.endGroup();
}


/* protected events */

void MainWindow::closeEvent(QCloseEvent *event)
{
    closeAllFiles();
    writeSettings();
    event->accept();
}


/* private slots */

void MainWindow::newFile()
{
    ++countNumber;
    WPWindow *window = createNewChild();
    window->setWindowTitle(tr("untitled %1[*]").arg(countNumber));
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
            recentFilesMenu->addRecentFile(file);
            return true;
        }
        else
        {
            statusBar()->showMessage(tr("Saving failed"), 2000);
            return false;
        }
    }
}

void MainWindow::closeFile()
{
    QMdiSubWindow *window = mdiArea->activeSubWindow();
    window->close();
}

void MainWindow::closeAllFiles()
{
    foreach (QMdiSubWindow *window, mdiArea->subWindowList())
        window->close();
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
    closeAction->setEnabled(hasActiveWindow);
    closeAllAction->setEnabled(hasActiveWindow);
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
        QString s = window->windowTitle();
        statusBar()->showMessage(s.left(s.size() - 3));
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
