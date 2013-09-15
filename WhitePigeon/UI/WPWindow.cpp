#include <QtWidgets>
#include <QtWebKitWidgets/QWebView>

// #include "musicscene.h"
#include <QGraphicsView>    // #include "musicview.h"
#include "WPScore/WPScore.h"
#include "core/WPSynthesisController.h"

#include "WPWindow.h"

WPWindow::WPWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMdiSubWindow(parent, flags)
{
    saved = false;
    filePath = QString();
    score = new WPScore;
    lastVersion = score->getCurrentVersion();
    mode = File;

    scene = new musicScene(this);
    // scene->setScore(score);
    // connect(scene, SIGNAL(scoreModified()),
    //         this, SLOT(onScoreModified()));QObject::connect: No such signal QWebView::urlChanged(QString)

    view = new musicView(this);
    view->setScene(scene);

    webView = new QWebView(this);
    connect(webView, SIGNAL(titleChanged(const QString &)),
        this, SLOT(setWindowTitle(const QString &)));
    connect(webView, SIGNAL(urlChanged(const QUrl &)),
        this, SLOT(changeFilePathInWebMode(const QUrl &)));

    setWidget(view);
    setAttribute(Qt::WA_DeleteOnClose);
}

WPWindow::Mode WPWindow::getMode() const
{
    return mode;
}

void WPWindow::setMode(Mode __mode)
{
    mode = __mode;
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
    saved = true;
    if (mode == File)
    {
        filePath = QFileInfo(file).canonicalFilePath();
        // score->load(filePath.toStdString());
        // scene->setScore(score);
        // lastVersion = score->getCurrentVersion();
        setWindowModified(false);
        setWindowTitle(QFileInfo(file).fileName() + "[*]");
        return true;
    }
    else
    {
        QUrl url(file);
        filePath = url.url();
        QWebSettings::setIconDatabasePath(QCoreApplication::applicationDirPath());
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
        webView->load(url);
		webView->page()->setLinkDelegationPolicy(QWebPage::DelegateExternalLinks);
        setWidget(webView);
        return true;
    }
}

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
    // score->save(filePath.toStdString());
    // lastVersion = score->getCurrentVersion();
    setWindowModified(false);
    return true;
}

bool WPWindow::saveFile(const QString &file)
{
    if (mode == Web)
    {
        return true;
    }
    saved = true;
    filePath = QFileInfo(file).canonicalFilePath();
    // score->save(filePath.toStdString());
    // lastVersion = score->getCurrentVersion();
    setWindowModified(false);
    setWindowTitle(QFileInfo(file).fileName() + "[*]");
    return true;
}

void WPWindow::play_with(WPSynthesisController *controller)
{
    // controller->synthesizeAndPlay();
}

void WPWindow::closeEvent(QCloseEvent *closeEvent)
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

void WPWindow::onScoreModified()
{
    setWindowModified(lastVersion != score->getCurrentVersion());
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
    emit loadFinished();
}

void WPWindow::refreshIcon()
{
    qDebug()<<"i change myslef; "<<webView->icon().name();
    setWindowIcon(webView->icon());
}

bool WPWindow::okToContinue()
{
    if (isWindowModified() && mode == File)
    {
        int r = QMessageBox::warning(this, tr("WhitePigeon"),
                        tr("The document has been modified.\n"
                           "Do you want to save your changes?"),
                        QMessageBox::Yes |
                        QMessageBox::No |
                        QMessageBox::Cancel);
        if (r == QMessageBox::Yes)
        {
            return saveFile();
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
