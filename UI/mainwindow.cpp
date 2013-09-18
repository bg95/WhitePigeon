#include <QtWidgets>

#include "QRecentFilesMenu.h"
#include "QRecentWebsitesMenu.h"
#include "versiondialog.h"
#include "core/WPSynthesisController.h"
#include "WPImage/Image_Processing.h"
#include "OscilloscopeWindow.h"
#include "WPWindow.h"
#include "musicshower/musicscene.h"

#include "mainwindow.h"

MainWindow::MainWindow()
{
    /* Private widget settings */
    controller = new WPSynthesisController(this);
    oscilloscopeWindow = new OscilloscopeWindow;
    countNumber = 0;
    mdiArea = new QMdiArea;
    mdiArea->setParent(this);
    mdiArea->setViewMode(QMdiArea::TabbedView);
    mdiArea->setActivationOrder(QMdiArea::CreationOrder);
	mdiArea->setTabsClosable(true);
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
            this, SLOT(updateActions()));

    /* MainWindow settings */
    setWindowTitle(tr("WhitePigeon"));
    setWindowIcon(QIcon(":/images/whitepigeon.png"));
    setContextMenuPolicy(Qt::ActionsContextMenu);

    /* UI settings */
    createActions();
    createMenus();
    createToolBar();
    createAddressBar();
    statusBar();
    createContextMenu();

    readSettings();
    updateActions();
    updateCompleter();
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
    connect(saveAction, SIGNAL(triggered()),
            this, SLOT(saveFile()));

    saveAsAction = new QAction(this);
    saveAsAction->setText(tr("Save &As..."));
    saveAsAction->setIcon(QIcon(":/images/saveas.gif"));
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    saveAsAction->setStatusTip(tr("Save the file under a new name"));
    saveAsAction->setToolTip(tr("Save the file under a new name"));
    connect(saveAsAction, SIGNAL(triggered()),
            this, SLOT(saveAsFile()));

    closeAction = new QAction(this);
    closeAction->setText(tr("Close"));
    closeAction->setIcon(QIcon(":/images/close.gif"));
    closeAction->setStatusTip(tr("Close the file"));
    closeAction->setToolTip(tr("Close the file"));
    connect(closeAction, SIGNAL(triggered()),
            this, SLOT(closeFile()));

    closeAllAction = new QAction(this);
    closeAllAction->setText(tr("Close All"));
    closeAllAction->setIcon(QIcon(":/images/closeall.gif"));
    // closeAllAction->setShortcut();
    closeAllAction->setStatusTip(tr("Close all files"));
    closeAllAction->setToolTip(tr("Close all files"));
    connect(closeAllAction, SIGNAL(triggered()),
            this, SLOT(closeAllFiles()));

    exitAction = new QAction(this);
    exitAction->setText(tr("E&xit"));
    exitAction->setIcon(QIcon(":/images/exit.png"));
    exitAction->setShortcut(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Exit the application"));
    exitAction->setToolTip(tr("Exit the application"));
    connect(exitAction, SIGNAL(triggered()),
            this, SLOT(close()));

    newPartAction = new QAction(this);
    newPartAction->setText(tr("New part"));
    newPartAction->setIcon(QIcon(":/images/newpart.png"));
    // newPartAction->setShortcut();
    newPartAction->setStatusTip(tr("Build a new part"));
    newPartAction->setToolTip(tr("Build a new part"));
    connect(newPartAction, SIGNAL(triggered()),
            this, SLOT(newPart()));

    undoAction = new QAction(this);
    undoAction->setText(tr("&Undo"));
    undoAction->setIcon(QIcon(":/images/undo.png"));
    undoAction->setShortcut(QKeySequence::Undo);
    undoAction->setStatusTip(tr("Undo"));
    undoAction->setToolTip(tr("Undo"));
    connect(undoAction, SIGNAL(triggered()),
            this, SLOT(undo()));

    redoAction = new QAction(this);
    redoAction->setText(tr("&Redo"));
    redoAction->setIcon(QIcon(":/images/redo.png"));
    redoAction->setShortcut(QKeySequence::Redo);
    redoAction->setStatusTip(tr("Redo"));
    redoAction->setToolTip(tr("Redo"));
    connect(redoAction, SIGNAL(triggered()),
            this, SLOT(redo()));

    refreshAction = new QAction(this);
    refreshAction->setText(tr("Refresh"));
    refreshAction->setIcon(QIcon(":/images/refresh.png"));
    refreshAction->setShortcut(QKeySequence::Refresh);
    refreshAction->setStatusTip(tr("Refresh"));
    refreshAction->setToolTip(tr("Refresh"));
    connect(refreshAction, SIGNAL(triggered()),
            this, SLOT(refresh()));

    playAction = new QAction(this);
    playAction->setText(tr("Play"));
    playAction->setIcon(QIcon(":/images/play.gif"));
    // playAction->setShortcut();
    playAction->setStatusTip(tr("Play the score"));
    playAction->setToolTip(tr("Play the score"));
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
    fileToolViewAction->setText(tr("Tool Bar"));
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
    oscilloscopeAction->setStatusTip(tr("Show an oscilloscope"));
    oscilloscopeAction->setToolTip(tr("Show an oscilloscope"));
    connect(oscilloscopeAction, SIGNAL(triggered()),
            this, SLOT(showOscilloscope()));

    imageRecogniseAction = new QAction(this);
    imageRecogniseAction->setText(tr("Image Recognition"));
    imageRecogniseAction->setIcon(QIcon(":/images/image.png"));
    imageRecogniseAction->setStatusTip(tr("Analyse an image and produce a score"));
    imageRecogniseAction->setToolTip(tr("Analyse an image and produce a score"));
    connect(imageRecogniseAction, SIGNAL(triggered()),
            this, SLOT(recongiseImage()));
}

void MainWindow::createMenus()
{
    /* Create all menus */
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);

    recentFilesMenu = new QRecentFilesMenu(fileMenu);
    recentFilesMenu->setTitle(tr("&Recent Files"));
    recentFilesMenu->setIcon(QIcon(":/images/recent.png"));
    connect(recentFilesMenu, SIGNAL(recentFileTriggered(const QString &)),
            this, SLOT(loadFile(const QString &)));

    recentWebsitesMenu = new QRecentWebsitesMenu(fileMenu);
    recentWebsitesMenu->setTitle(tr("History visit websites"));
    recentWebsitesMenu->setIcon(QIcon(":/images/web.gif"));
    connect(recentWebsitesMenu, SIGNAL(recentWebsiteTriggered(const QString &)),
            this, SLOT(loadFile(const QString &)));

    fileMenu->addMenu(recentFilesMenu);
    fileMenu->addMenu(recentWebsitesMenu);
    fileMenu->addSeparator();
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(closeAction);
    fileMenu->addAction(closeAllAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(newPartAction);
    editMenu->addSeparator();
    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);

    versionMenu = new QMenu(editMenu);
    versionMenu->setTitle(tr("Load/Save Version"));
    connect(versionMenu, SIGNAL(triggered(QAction*)),
            this, SLOT(switchVersion(QAction*)));
    editMenu->addMenu(versionMenu);

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
    toolsMenu->addAction(imageRecogniseAction);
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
    toolToolBar->addAction(imageRecogniseAction);
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
    undoButton = new QPushButton(this);
    undoButton->setIcon(QIcon(":/images/undo.png"));
    undoButton->setFlat(true);
    connect(undoButton, SIGNAL(clicked()),
            this, SLOT(undo()));

    redoButton = new QPushButton(this);
    redoButton->setIcon(QIcon(":/images/redo.png"));
    redoButton->setFlat(true);
    connect(redoButton, SIGNAL(clicked()),
            this, SLOT(redo()));

    refreshButton = new QPushButton(this);
    refreshButton->setIcon(QIcon(":/images/refresh.png"));
    refreshButton->setFlat(true);
    connect(refreshButton, SIGNAL(clicked()),
            this, SLOT(refresh()));

    completer = new QCompleter;
    completer->setMaxVisibleItems(5);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    connect(recentFilesMenu, SIGNAL(recentFilesModified()),
            this, SLOT(updateCompleter()));
    connect(recentWebsitesMenu, SIGNAL(recentWebsitesModified()),
            this, SLOT(updateCompleter()));

    addressEdit = new QLineEdit(this);
    addressEdit->setCompleter(completer);
    connect(addressEdit, SIGNAL(returnPressed()),
            this, SLOT(goToSite()));
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
            this, SLOT(updateAddressBar()));

	goButton = new QPushButton(this);
    goButton->setIcon(QIcon(":/images/go.gif"));
	goButton->setFlat(true);
    connect(goButton, SIGNAL(clicked()),
            this, SLOT(goToSite()));

    addressLayout = new QHBoxLayout;
    addressLayout->setSpacing(0);
    addressLayout->addWidget(undoButton);
    addressLayout->addWidget(redoButton);
    addressLayout->addWidget(refreshButton);
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

    settings.beginGroup("RecentWebsites");
    recentWebsitesMenu->restoreState(settings.value("websites").toByteArray());
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

    settings.beginGroup("RecentWebsites");
    settings.setValue("websites", recentWebsitesMenu->saveState());
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
    WPWindow *window = createNewChildFileMode();
    window->setWindowTitle(tr("untitled %1[*]").arg(countNumber));
    window->show();
}

void MainWindow::loadFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                               tr("Open a file"), ".",
                               tr("Score files (*.wps)"));
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
            window = createNewChildFileMode();
            if (window->loadFile(file))
            {
                window->show();
                mdiArea->setActiveSubWindow(window);
                recentFilesMenu->addRecentFile(fileInfo.canonicalFilePath());
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
            if (url.scheme() == "http" || url.scheme() == "https")
			{
                WPWindow *window = createNewChildWebMode();
				window->loadFile(url.url());
                window->show();
                mdiArea->setActiveSubWindow(window);
            }
            else
            {
                QMessageBox::warning(this, tr("White Pigeon"),
                        tr("Url must begin with \"http://\" or \"https://\""),
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
    undoButton->setVisible(visible);
    redoButton->setVisible(visible);
    refreshButton->setVisible(visible);
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
            qDebug() << "Here 1";
            statusBar()->showMessage(tr("File saved"), 2000);
            return true;
        }
        else
        {
            qDebug() << "Here 2";
            statusBar()->showMessage(tr("Saving failed"), 2000);
            return false;
        }
    }
    else
    {
        qDebug() << "Here";
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
            recentFilesMenu->addRecentFile(QFileInfo(file).canonicalFilePath());
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

void MainWindow::newPart()
{
    WPWindow *window = dynamic_cast<WPWindow *> (mdiArea->activeSubWindow());
    window->newPart();
}

void MainWindow::undo()
{
    WPWindow *window = dynamic_cast<WPWindow *> (mdiArea->activeSubWindow());
    if (window)
    {
        window->undo();
    }
}

void MainWindow::redo()
{
    WPWindow *window = dynamic_cast<WPWindow *> (mdiArea->activeSubWindow());
    if (window)
    {
        window->redo();
    }
}

void MainWindow::switchVersion(QAction *action)
{
    if (action->data().isValid())
    {
        WPWindow *window = dynamic_cast<WPWindow *> (mdiArea->activeSubWindow());
        window->switchVersion(action->data().toInt());
    }
}

void MainWindow::recordCurrentVersion()
{
    VersionDialog *dialog = new VersionDialog(this);
    if (dialog->exec())
    {
        WPWindow *window = dynamic_cast<WPWindow *> (mdiArea->activeSubWindow());
        int index = dialog->getNumber() - 1;
        QString information = dialog->getInformation();
        window->version[index] = window->score->getCurrentVersion();
        window->enable[index] = true;
        window->tip[index] = information;
        updateVersionMenu(window);
    }
    delete dialog;
}

void MainWindow::refresh()
{
    WPWindow *window = dynamic_cast<WPWindow *> (mdiArea->activeSubWindow());
    if (window)
    {
        window->refresh();
    }
}

void MainWindow::play()
{
    WPWindow *window = dynamic_cast<WPWindow *>(mdiArea->activeSubWindow());
    window->play_with(controller);
}

void MainWindow::stopAll()
{
    controller->stopAll();
}

void MainWindow::updateActions()
{
    WPWindow *window = dynamic_cast<WPWindow *> (mdiArea->activeSubWindow());
    bool hasActiveWindow = (window != NULL);
    bool isWindowFileMode = (window != NULL && window->getMode() == WPWindow::File);

    saveAction->setEnabled(isWindowFileMode);
    saveAsAction->setEnabled(isWindowFileMode);
    newPartAction->setEnabled(isWindowFileMode);
    playAction->setEnabled(isWindowFileMode);
    closeAction->setEnabled(hasActiveWindow);
    closeAllAction->setEnabled(hasActiveWindow);

    updateVersionMenu(window);
}

void MainWindow::updateVersionMenu(WPWindow *window)
{
    bool isWindowFileMode = (window != NULL && window->getMode() == WPWindow::File);

    versionMenu->setEnabled(isWindowFileMode);
    versionMenu->clear();
    if (isWindowFileMode)
    {
        for (int i = 0; i < 5; ++i)
        {
            QAction *versionAction = new QAction(versionMenu);
            versionAction->setText(tr("Version %1").arg(i + 1));
            versionAction->setData(window->version[i]);
            versionAction->setEnabled(window->enable[i]);
            versionAction->setStatusTip(window->tip[i]);
            versionMenu->addAction(versionAction);
        }
        versionMenu->addSeparator();
        versionMenu->addAction(tr("Record Current Version"),
                               this, SLOT(recordCurrentVersion()));
    }
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

void MainWindow::updateCompleter()
{
    QStringList list = recentFilesMenu->m_files;
    for (QList<WebHistory>::Iterator iter = recentWebsitesMenu->m_list.begin();
         iter != recentWebsitesMenu->m_list.end();
         ++iter)
    {
        QString str = iter->first;
        if (str.left(7) == "http://")
        {
            str.remove(0, 7);
        }
        list << str;
    }
    completer->setModel(new QStringListModel(list));
}

void MainWindow::onLoadFinished(const QPair<QString, QString> &history)
{
    statusBar()->clearMessage();
    updateActions();
    if (history.second != tr("Problem loading page"))
    {
        recentWebsitesMenu->addRecentWebsite(history);
    }
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
        oscilloscopeWindow = new OscilloscopeWindow(this);
    }
    oscilloscopeWindow->show();
}

void MainWindow::recongiseImage()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                               tr("Open an image file"), ".");
    if (fileName.isEmpty())
        return;
    QString filePath = QFileInfo(fileName).canonicalFilePath();
    Image_Processing process;
    if (!process.Main_Process(filePath))
    {
        QMessageBox::warning(this, tr("WhitePigeon"),
                             tr("Cannot recognise the file"),
                             QMessageBox::Ok);
        return;
    }
    WPWindow *window = createNewChildFileMode();
    window->score = process.Save_Data();
    window->scene->setScore(window->score);
    ++countNumber;
    window->setWindowTitle(tr("untitled %1[*]").arg(countNumber));
    window->setWindowModified(true);
    window->show();
}


/* private functions */

WPWindow* MainWindow::createNewChildFileMode()
{
    WPWindow *window = new WPWindow;
    mdiArea->addSubWindow(window);
    return window;
}

WPWindow* MainWindow::createNewChildWebMode()
{
    WPWindow *window = new WPWindow(WPWindow::Web);
    connect(window, SIGNAL(pathModified()),
            this, SLOT(updateAddressBar()));
    connect(window, SIGNAL(loadProgress(int)),
            this, SLOT(showLoadingProgress(int)));
    connect(window, SIGNAL(statusBarMessage(const QString &)),
            this, SLOT(showStatusBarMessage(const QString &)));
    connect(window, SIGNAL(linkClicked(const QUrl &)),
            this, SLOT(openAWebPage(const QUrl &)));
    connect(window, SIGNAL(loadFinished(const QPair<QString, QString> &)),
            this, SLOT(onLoadFinished(const QPair<QString, QString> &)));
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
