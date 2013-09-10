musicRepeatItem::musicRepeatItem() : repeatHeight(30) {
}

musicRepeatItem::~musicRepeatItem() {
}

void setHeight(qreal height) {
  repeatHeight = height;
}

QRectF boundingRect() const {
  return QRectF(-4, -repeatHeight/ 2, 8, repeatHeight);
}

QRectF paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
	     QWidget *widget) {
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
