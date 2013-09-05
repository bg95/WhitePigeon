#ifndef MUSICLINEITEM_H
#define MUSICLINEITEM_H

#include <QGraphicsLineItem>
//#include <QList>

class musicTextItem;

class musicLineItem : public QGraphicsItem
{

public:
	enum position
	{
        verticle,
        horizontal
    };
    musicLineItem(qreal length, position pos);
    ~musicLineItem();
	QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setColor(const QColor &color);
    //trackNodes();

private:
	qreal Length;
	position Pos;
	QColor lineColor;
    //musicTextItem *leftparent;
    //musicTextItem *rightparent;
};

#endif // MUSICLINEITEM_H
