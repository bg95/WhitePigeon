#include <QtWidgets>
#include <cmath>
#include "musicscene.h"
#include "musicview.h"

musicView::musicView(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent)
{
    setDragMode(ScrollHandDrag);
    //centerOn(scene->sceneRect().width() / 4, scene->sceneRect().height() / 4);
}

musicView::musicView(QWidget *parent) : QGraphicsView(parent) {
  setDragMode(ScrollHandDrag);
}

void musicView::setScene(QGraphicsScene *scene)
{
    QGraphicsView::setScene(scene);
    //centerOn(scene->sceneRect().width() / 4, scene->height() / 4);
}

void musicView::wheelEvent(QWheelEvent *event) {
  double numDegrees = -event->delta() / 8.0;
  double numSteps = numDegrees / 15.0;
  double factor = std::pow(1.125, numSteps);
  scale(factor, factor);
}
