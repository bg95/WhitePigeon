<<<<<<< HEAD
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
=======
#include <qmath.h>
#include <QtWidgets>

#include "musicview.h"

musicView::musicView(QWidget *parent)
    : QGraphicsView(parent),
      zoomFactor(1.0)
{
    setDragMode(ScrollHandDrag);
}

musicView::~musicView()
{
}

void musicView::wheelEvent(QWheelEvent *event)
{
    double numDegrees = event->delta() / 8.0;
    double numSteps = numDegrees / 15.0;
    double factor = qPow(1.125, numSteps);
    if (zoomFactor * factor > 5.0)
    {
        factor = 5.0 / zoomFactor;
        zoomFactor = 5.0;
        scale(factor, factor);
        return;
    }
    if (zoomFactor * factor < 1.0)
    {
        factor = 1.0 / zoomFactor;
        zoomFactor = 1.0;
        scale(factor, factor);
        return;
    }
    zoomFactor *= factor;
    scale(factor, factor);
>>>>>>> 02fefffa89294ac3b3f75f36a28e78d414bb2cc9
}
