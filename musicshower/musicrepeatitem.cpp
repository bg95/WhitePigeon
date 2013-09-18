#include <QtWidgets>
#include "musicrepeatitem.h"

musicRepeatItem::musicRepeatItem(direction dir)
    : repeatHeight(30), Dir(dir) {
}

musicRepeatItem::~musicRepeatItem() {
}

void musicRepeatItem::setHeight(qreal height) {
    repeatHeight = height;
}

QRectF musicRepeatItem::boundingRect() const {
  return QRectF(-4, -repeatHeight/ 2, 8, repeatHeight);
}

void musicRepeatItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
	     QWidget *widget) {
    if (Dir == positive)
    {
        QPen pen;
        pen.setWidth(3);
        painter->drawLine(-2.5, -repeatHeight/ 2, -2.5, repeatHeight/ 2);
        pen.setWidth(1);
        painter->drawLine(0.5, -repeatHeight/ 2, 0.5, repeatHeight/ 2);
        painter->setBrush(QBrush(Qt::black));
        QPointF upheart(2, repeatHeight/ 6);
        QPointF downheart(2, -repeatHeight/ 6);
        QRectF uprect(upheart.x() - 1, upheart.y() - 1, 2, 2);
        QRectF downrect(downheart.x() - 1, downheart.y() - 1, 2, 2);
        painter->drawEllipse(uprect);
        painter->drawEllipse(downrect);
    }
    else {
        QPen pen;
        pen.setWidth(3);
        painter->drawLine(2.5, -repeatHeight/ 2, 2.5, repeatHeight/ 2);
        pen.setWidth(1);
        painter->drawLine(-0.5, -repeatHeight/ 2, -0.5, repeatHeight/ 2);
        painter->setBrush(QBrush(Qt::black));
        QPointF upheart(-2, repeatHeight/ 6);
        QPointF downheart(-2, -repeatHeight/ 6);
        QRectF uprect(upheart.x() - 1, upheart.y() - 1, 2, 2);
        QRectF downrect(downheart.x() - 1, downheart.y() - 1, 2, 2);
        painter->drawEllipse(uprect);
        painter->drawEllipse(downrect);

    }
}
