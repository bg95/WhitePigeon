#include "musicarcitem.h"

#include <QtWidgets>

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
