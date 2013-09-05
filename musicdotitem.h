#ifndef MUSICDOTITEM_H
#define MUSICDOTITEM_H

#include <QGraphicsItem>

class musicTextItem;

class musicDotItem : public QGraphicsItem
{
public:
    musicDotItem();
    ~musicDotItem();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setColor(const QColor &color);
    void setRadius(const qreal);
    qreal radius();
    //void track();

private:
    QColor dotColor;
    qreal dotRadius;
    //position dotPos;
    //musicTextItem *parentNumber;
};

#endif // MUSICDOTITEM_H
