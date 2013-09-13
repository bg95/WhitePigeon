#include <QtWidgets>
#include <QtWebKitWidgets/QWebView>

#include "musicshower/musicscene.h"
#include "musicshower/musicview.h"
#include "WPScore/WPScore.h"
#include "core/WPSynthesisController.h"

#include "WPWindow.h"

WPWindow::WPWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMdiSubWindow(parent, flags),
      saved(false),
      mode(File),
      filePath(),
      score(0),
      lastVersion(0),
      view(0),
      scene(0),
      webView(0)
{
    score = new WPScore;
    lastVersion = score->getCurrentVersion();

    scene = new musicScene(this);
	scene->setScore(score);

    view = new musicView(this);
    view->setScene(scene);

    for (int i = 0; i < 5; ++i)
    {
        version[i] = 0;
        enable[i] = false;
        tip[i].clear();
    }

    setWidget(view);
    setAttribute(Qt::WA_DeleteOnClose);
}

WPWindow::WPWindow(Mode __mode, QWidget *parent, Qt::WindowFlags flags)
    : QMdiSubWindow(parent, flags),
      saved(false),
      mode(__mode),
      filePath(),
      score(0),
      lastVersion(0),
      view(0),
      scene(0),
      webView(0)
{
    if (mode == Web)
    {
        QWebSettings::setIconDatabasePath(QCoreApplication::applicationDirPath());
        webView = new QWebView(this);
        connect(webView, SIGNAL(titleChanged(const QString &)),
            this, SLOT(setWindowTitle(const QString &)));
        connect(webView, SIGNAL(urlChanged(const QUrl &)),
            this, SLOT(changeFilePathInWebMode(const QUrl &)));
        connect(webView, SIGNAL(loadProgress(int)),
                this, SLOT(onLoadProgress(int)));
        connect(webView, SIGNAL(statusBarMessage(QString)),
                this, SLOT(onStatusBarMessage(const QString &)));
        connect(webView, SIGNAL(linkClicked(QUrl)),
                this, SLOT(onLinkClicked(const QUrl &)));
        connect(webView, SIGNAL(iconChanged()),
                this, SLOT(refreshIcon()));
        connect(webView, SIGNAL(loadFinished(bool)),
                this, SLOT(loadFailure(bool)));
        webView->page()->setLinkDelegationPolicy(QWebPage::DelegateExternalLinks);

        setWidget(webView);
        setAttribute(Qt::WA_DeleteOnClose);
    }
    else
    {
        score = new WPScore;
        lastVersion = score->getCurrentVersion();

        scene = new musicScene(this);
        scene->setScore(score);

        view = new musicView(this);
        view->setScene(scene);

        for (int i = 0; i < 5; ++i)
        {
            version[i] = 0;
            enable[i] = false;
            tip[i].clear();
        }

        setWidget(view);
        setAttribute(Qt::WA_DeleteOnClose);
    }
}

WPWindow::~WPWindow()
{
    if (mode == File)
    {
        score->lockForWrite();
        score->close();
        // tell other to stop
        delete score;
        delete view;
        delete scene;
    }
    else
    {
        delete webView;
    }
}

WPWindow::Mode WPWindow::getMode() const
{
    return mode;
}

void WPWindow::setMode(Mode __mode)
{
    mode = __mode;
    if (mode == Web)
    {
        webView->show();
    }
}

bool WPWindow::isSaved() const
{
    return saved;
}

QString WPWindow::currentFilePath() const
{
    return filePath;
}

bool WPWindow::loadFile(const QString &file)
{
    if (mode == File)
    {
        filePath = QFileInfo(file).canonicalFilePath();
        int result = 0;
		score->lockForWrite();
        result = score->load(filePath.toStdString());
        saved = true;
        lastVersion = score->getCurrentVersion();
        score->unlock();
		scene->setScore(score);
        setWindowModified(false);
        setWindowTitle(QFileInfo(file).fileName() + "[*]");
        return result == 0;
    }
    else
    {
        QUrl url(file);
        filePath = url.url();
        webView->load(url);
        return true;
    }
}

// save the file already existing
bool WPWindow::saveFile()
{
    if (mode == Web)
    {
        return true;
    }
    if (!saved)
    {
        return false;
    }
	score->lockForRead();
	score->save(filePath.toStdString());
	lastVersion = score->getCurrentVersion();
	score->unlock();
    setWindowModified(false);
    return true;
}

// save as a new file using "file"
bool WPWindow::saveFile(const QString &file)
{
    if (mode == Web)
    {
        return true;
    }
	saved = true;
	score->lockForRead();
	score->save(file.toStdString());
	lastVersion = score->getCurrentVersion();
	score->unlock();
	filePath = QFileInfo(file).canonicalFilePath();
    setWindowTitle(QFileInfo(file).fileName() + "[*]");
    setWindowModified(false);
    return true;
}

void WPWindow::newPart()
{
    score->lockForWrite();
    score->newVersion();
    score->newPart();
    setWindowModified(score->getCurrentVersion() != lastVersion);
    score->unlock();
    scene->setScore(score);
}

void WPWindow::undo()
{
    if (mode == Web)
    {
        webView->back();
    }
    else
    {
        score->lockForWrite();
        score->undo();
        setWindowModified(lastVersion != score->getCurrentVersion());
        score->unlock();
        scene->setScore(score);
    }
}

void WPWindow::redo()
{
    if (mode == Web)
    {
        webView->forward();
    }
    else
    {
        score->lockForWrite();
        score->redo();
        setWindowModified(lastVersion != score->getCurrentVersion());
        score->unlock();
        scene->setScore(score);
    }
}

void WPWindow::switchVersion(int version)
{
    score->lockForWrite();
    score->switchVersion(version);
    setWindowModified(lastVersion != score->getCurrentVersion());
    score->unlock();
    scene->setScore(score);
}

void WPWindow::refresh()
{
    if (mode == Web)
    {
        webView->stop();
        webView->load(QUrl(filePath));
    }
    else
    {
        scene->setScore(score);
    }
}

void WPWindow::play_with(WPSynthesisController *controller)
{
    // controller->synthesizeAndPlay(*score);
}

void WPWindow::closeEvent(QCloseEvent *closeEvent)
{
    if (mode == Web)
    {
        webView->stop();
        webView->close();
        closeEvent->accept();
    }
    else
    {
        if (okToContinue())
        {
            closeEvent->accept();
        }
        else
        {
            closeEvent->ignore();
        }
    }
}

void WPWindow::changeFilePathInWebMode(const QUrl &url)
{
    filePath = url.url();
    emit pathModified();
}

void WPWindow::onLoadProgress(int progress)
{
    emit loadProgress(progress);
}

void WPWindow::onStatusBarMessage(const QString &msg)
{
    emit statusBarMessage(msg);
}

void WPWindow::onLinkClicked(const QUrl &url)
{
    emit linkClicked(url);
}

void WPWindow::loadFailure(bool Flag)
{
	if (!Flag)
    {
        setWindowTitle("Problem loading page");
    }
    emit loadFinished(QPair<QString, QString>(webView->url().url(), windowTitle()));
}

void WPWindow::refreshIcon()
{
    setWindowIcon(webView->icon());
}

bool WPWindow::okToContinue()
{
    if (isWindowModified())
    {
        int r = QMessageBox::warning(this, tr("WhitePigeon"),
                        tr("The document has been modified.\n"
                           "Do you want to save your changes?"),
                        QMessageBox::Yes |
                        QMessageBox::No |
                        QMessageBox::Cancel);
        if (r == QMessageBox::Yes)
        {
            if (saved)
            {
                return saveFile();
            }
            else
            {
                QString file = QFileDialog::getSaveFileName(this, tr("Save as"));
                return saveFile(file);
            }
        }
        else
        {
            if (r == QMessageBox::Cancel)
            {
                return false;
            }
            else
            {
                return true;
            }
        }
    }
    else
    {
        return true;
    }
}
