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
}
