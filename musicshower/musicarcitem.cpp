#include "musicarcitem.h"

#include <QtWidgets>

musicArcItem::musicArcItem(QPointF start, QPointF end, QGraphicsItem *parent)
    :QGraphicsPathItem(parent)
{
    qreal width = end.x() - start.x();
    qreal pointy = start.y() - width * 1.0 / 8;
    //qreal centery = start.y() - width / 8;
    QPainterPath arcPath;
    arcPath.moveTo(end);
    arcPath.arcTo(start.x(), pointy, width, width * 1.0 / 4, 0, 180);
    setPath(arcPath);
}
