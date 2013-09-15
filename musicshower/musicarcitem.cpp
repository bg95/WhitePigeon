#include "musicarcitem.h"

#include <QtWidgets>

/*
musicArcItem::musicArcItem(QPointF st)
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
*/

musicArcItem::musicArcItem(qreal span) : arcSpan(span) {}
QRectF musicArcItem::boundingRect() const {
  qreal height = arcSpan / 4;
  return QRectF(-arcSpan / 2, - height / 2, arcSpan, height);
}

QPainterPath musicArcItem::shape() const {
}

void musicArcItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
	   QWidget *widget) {
  QPainterPath path;
  qreal height = arcSpan / 4;
  path.moveTo(-arcSpan / 2, height / 2);
  path.arcTo(-arcSpan / 2, -height / 2, arcSpan, arcSpan / 2, 0, 180);
  painter->drawPath(path);
}

musicArcItem::~musicArcItem() {
}
