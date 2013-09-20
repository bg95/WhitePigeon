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
    mdiArea->setTabsMovable(true);
    mdiArea->setActivationOrder(QMdiArea::CreationOrder);
	mdiArea->setTabsClosable(true);
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
            this, SLOT(updateActionsNeedingSubWindow()));

    /* MainWindow settings */
    setWindowTitle(tr("WhitePigeon"));
    // setWindowIcon(QIcon(":/images/WhitePigeon.jpg"));
    setContextMenuPolicy(Qt::ActionsContextMenu);

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
    newAction->setIcon(QIcon(":/images/new.gif"));
    newAction->setShortcut(QKeySequence::New);
    newAction->setStatusTip(tr("Create a new file"));
    newAction->setToolTip(tr("Create a new file"));
    connect(newAction, SIGNAL(triggered()),
            this, SLOT(newFile()));

    openAction = new QAction(this);
    openAction->setText(tr("&Open..."));
    openAction->setIcon(QIcon(":/images/open.gif"));
    openAction->setShortcut(QKeySequence::Open);
    openAction->setStatusTip(tr("Open an existing file"));
    openAction->setToolTip(tr("Open an existing file"));
    connect(openAction, SIGNAL(triggered()),
            this, SLOT(loadFile()));

    saveAction = new QAction(this);
    saveAction->setText(tr("&Save"));
    saveAction->setIcon(QIcon(":/images/save.gif"));
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setStatusTip(tr("Save the file to disk"));
    saveAction->setToolTip(tr("Save the file"));
    saveAction->setEnabled(false);
    connect(saveAction, SIGNAL(triggered()),
            this, SLOT(saveFile()));

    saveAsAction = new QAction(this);
    saveAsAction->setText(tr("Save &As..."));
    saveAsAction->setIcon(QIcon(":/images/saveas.gif"));
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    saveAsAction->setStatusTip(tr("Save the file under a new name"));
    saveAsAction->setToolTip(tr("Save the file under a new name"));
    saveAsAction->setEnabled(false);
    connect(saveAsAction, SIGNAL(triggered()),
            this, SLOT(saveAsFile()));

    closeAction = new QAction(this);
    closeAction->setText(tr("Close"));
    closeAction->setIcon(QIcon(":/images/close.gif"));
    closeAction->setStatusTip(tr("Close the file"));
    closeAction->setToolTip(tr("Close the file"));
    closeAction->setEnabled(false);
    connect(closeAction, SIGNAL(triggered()),
            this, SLOT(closeFile()));

    closeAllAction = new QAction(this);
    closeAllAction->setText(tr("Close All"));
    closeAllAction->setIcon(QIcon(":/images/closeall.gif"));
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
    playAction->setIcon(QIcon(":/images/play.gif"));
    // playAction->setShortcut();
    playAction->setStatusTip(tr("Play the score"));
    playAction->setToolTip(tr("Play the score"));
    playAction->setEnabled(false);
    connect(playAction, SIGNAL(triggered()),
            this, SLOT(play()));

    stopAction = new QAction(this);
    stopAction->setText(tr("Stop"));
    stopAction->setIcon(QIcon(":/images/stop.gif"));
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
    oscilloscopeAction->setIcon(QIcon(":/images/oscilloscope.png"));
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
    addressEdit = new QLineEdit(this);
    connect(addressEdit, SIGNAL(returnPressed()),
            this, SLOT(goToSite()));
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
            this, SLOT(updateAddressBar()));

    goButton = new QPushButton(this);
    goButton->setIcon(QIcon(":/images/go.gif"));
    // goButton->setFlat(true);
    connect(goButton, SIGNAL(clicked()),
            this, SLOT(goToSite()));

    addressLayout = new QHBoxLayout;
    addressLayout->setSpacing(0);
    addressLayout->addWidget(addressEdit);
    addressLayout->addWidget(goButton);
    connect(addressViewAction, SIGNAL(triggered(bool)),
            this, SLOT(setAddressVisible(bool)));

    mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addLayout(addressLayout);
    mainLayout->addWidget(mdiArea);

    widget = new QWidget(this);
    widget->setLayout(mainLayout);
    addressEdit->setParent(widget);
    goButton->setParent(widget);
    mdiArea->setParent(widget);
    setCentralWidget(widget);
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
    setAddressVisible(settings.value("visible").toBool());
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
    settings.setValue("visible", addressViewAction->isChecked());
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
	QUrl url = QUrl::fromUserInput(file);
    QFileInfo fileInfo(file);
    if (fileInfo.isFile())
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
    else
    {
        if (url.isValid())
        {
			if (url.scheme() == "http")
			{
                WPWindow *window = createNewChild();
                window->setMode(WPWindow::Web);
				window->loadFile(url.url());
                window->show();
                mdiArea->setActiveSubWindow(window);
            }
            else
            {
                QMessageBox::warning(this, tr("White Pigeon"),
                        tr("Url must begin with \"http://\""),
                        QMessageBox::Ok);
            }
        }
        else
        {
            QMessageBox::warning(this, tr("White Pigeon"),
                    tr("Cannot find the file or website"),
                    QMessageBox::Ok);
            statusBar()->showMessage(tr("Illegal file path or url"), 2000);
        }
    }
}

void MainWindow::goToSite()
{
    loadFile(addressEdit->text());
}

void MainWindow::openAWebPage(const QUrl &url)
{
    loadFile(url.url());
}

void MainWindow::setAddressVisible(bool visible)
{
    addressEdit->setVisible(visible);
    goButton->setVisible(visible);
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

void MainWindow::updateAddressBar()
{
    WPWindow *window = dynamic_cast<WPWindow *> (mdiArea->activeSubWindow());
    if (window)
    {
        addressEdit->setText(window->currentFilePath());
    }
    else
    {
        addressEdit->setText(tr(""));
    }
}

void MainWindow::onLoadFinished()
{
    statusBar()->showMessage(tr(""), 1);
}

void MainWindow::updateActionsNeedingSubWindow()
{
    WPWindow *window = dynamic_cast<WPWindow *> (mdiArea->activeSubWindow());
    bool hasActiveWindow = (window != NULL);
    bool isWindowFileMode = (window != NULL && window->getMode() == WPWindow::File);

    saveAction->setEnabled(isWindowFileMode);
    saveAsAction->setEnabled(isWindowFileMode);
    playAction->setEnabled(isWindowFileMode);
    closeAction->setEnabled(hasActiveWindow);
    closeAllAction->setEnabled(hasActiveWindow);
}

void MainWindow::showLoadingProgress(int percent)
{
    statusBar()->showMessage(tr("Loading %1%").arg(percent));
}

void MainWindow::showStatusBarMessage(const QString &msg)
{
    statusBar()->showMessage(msg, 2000);
}

void MainWindow::showOscilloscope()
{
    if (!oscilloscopeWindow)
    {
        // oscilloscopeWindow = new OscilloscopeWindow(this);
    }
    oscilloscopeWindow->show();
}


/* private functions */

WPWindow* MainWindow::createNewChild()
{
    WPWindow *window = new WPWindow;
    connect(window, SIGNAL(pathModified()),
            this, SLOT(updateAddressBar()));
    connect(window, SIGNAL(loadProgress(int)),
            this, SLOT(showLoadingProgress(int)));
    connect(window, SIGNAL(statusBarMessage(const QString &)),
            this, SLOT(showStatusBarMessage(const QString &)));
    connect(window, SIGNAL(linkClicked(const QUrl &)),
            this, SLOT(openAWebPage(const QUrl &)));
    connect(window, SIGNAL(loadFinished()),
            this, SLOT(onLoadFinished()));
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