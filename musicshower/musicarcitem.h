#ifndef MUSICARCITEM_H
#define MUSICARCITEM_H

#include <QGraphicsPathItem>

class musicArcItem : public QGraphicsItem
{
    //Q_OBJECT
 public:
  musicArcItem(qreal span);
  ~musicArcItem();
  QRectF boundingRect() const;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
	     QWidget *widget);
  QPainterPath shape() const;
 private:
  qreal arcSpan;
 
};

#endif // MUSICARCITEM_H
