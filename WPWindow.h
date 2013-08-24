#ifndef WPWINDOW_H
#define WPWINDOW_H

#include <QMdiSubWindow>
#include <QGraphicsScene>   // Deleted
#include <QGraphicsView>    // Deleted

typedef QGraphicsScene WPGraphicsScene; // class WPGraphicsScene;
typedef QGraphicsView WPGraphicsView;   // class WPGraphicsView;

class WPWindow : public QMdiSubWindow
{
public:
    WPWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);

private:
    WPGraphicsView *view;
    WPGraphicsScene *scene;
};

#endif // WPWINDOW_H
