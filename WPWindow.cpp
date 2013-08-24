#include <QGraphicsScene>   // #include "WPGraphicsScene.h"
#include <QGraphicsView>    // #include "WPGraphicsView.h"
#include <QFileInfo>
#include "core/WPSynthesisController.h"

#include "WPWindow.h"

WPWindow::WPWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMdiSubWindow(parent, flags)
{
    saved = false;
    filePath = QString();

    scene = new WPGraphicsScene;
    scene->setParent(this);

    view = new WPGraphicsView;
    view->setParent(this);
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
    setWindowTitle(QFileInfo(file).fileName());
    return true;
}

bool WPWindow::saveFile()
{
    if (!saved)
    {
        return false;
    }
    // Write the file
    return true;
}

bool WPWindow::saveFile(const QString &file)
{
    // streaming out to the file
    saved = true;
    filePath = QFileInfo(file).canonicalFilePath();
    setWindowTitle(QFileInfo(file).fileName());
    return true;
}

void WPWindow::play_with(WPSynthesisController *controller)
{
    // controller->synthesizeAndPlay();
}
