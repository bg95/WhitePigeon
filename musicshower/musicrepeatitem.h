#ifndef MUSICREPEATITEM_H
#define MUSICREPEATITEM_H

#include <QGraphicsItem>

class musicRepeatItem : public QGraphicsItem
{
 public:
  musicRepeatItem();
  ~musicRepeatItem();
  void setHeight(qreal height);
  QRectF boundingRect() const;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, 
	     QWidget *widget);
 private:
  qreal repeatHeight;
};
