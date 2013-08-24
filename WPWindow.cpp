#include <QGraphicsScene>   // #include "WPGraphicsScene.h"
#include <QGraphicsView>    // #include "WPGraphicsView.h"
#include <QFileInfo>

#include "WPWindow.h"

WPWindow::WPWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMdiSubWindow(parent, flags)
{
    filePath = QString();

    scene = new WPGraphicsScene;
    scene->setParent(this);

    view = new WPGraphicsView;
    view->setParent(this);
    view->setScene(scene);

    setWidget(view);
    setAttribute(Qt::WA_DeleteOnClose);
}

bool WPWindow::loadFile(const QString &file)
{
    filePath = QFileInfo(file).canonicalFilePath();
    setWindowTitle(QFileInfo(file).fileName());
    return true;
}

QString WPWindow::currentFilePath() const
{
    return filePath;
}
