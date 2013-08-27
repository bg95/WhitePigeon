#include <QGraphicsScene>   // #include "WPGraphicsScene.h"
#include <QGraphicsView>    // #include "WPGraphicsView.h"

#include "WPWindow.h"

WPWindow::WPWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMdiSubWindow(parent, flags)
{
    scene = new WPGraphicsScene;
    scene->setParent(this);

    view = new WPGraphicsView;
    view->setParent(this);
    view->setScene(scene);

    setWidget(view);
    setAttribute(Qt::WA_DeleteOnClose);
}
