#include <QtWidgets>

#include "musicscene.h"
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

    scene = new musicScene(this);
    // scene->setScore(score);
    // connect(scene, SIGNAL(scoreModified()),
    //         this, SLOT(onScoreModified()));

    view = new musicView(this);
    view->setScene(scene);

    setWidget(view);
    setAttribute(Qt::WA_DeleteOnClose);
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
    filePath = QFileInfo(file).canonicalFilePath();
    // score->load(filePath.toStdString());
    // scene->setScore(score);
    // lastVersion = score->getCurrentVersion();
    setWindowModified(false);
    setWindowTitle(QFileInfo(file).fileName() + "[*]");
    return true;
}

bool WPWindow::saveFile()
{
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
