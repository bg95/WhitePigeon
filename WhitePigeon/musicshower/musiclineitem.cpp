#include "musiclineitem.h"
#include "musictextitem.h"

#include <QtWidgets>

musicLineItem::musicLineItem(qreal length, position pos) 
	: Length(length), Pos(pos), lineColor(Qt::black)
{

    //setZValue(-1);
    /*
    foreach (musicTextItem *item, parents)
    {
        item->addLine(this);
    }
    */
    //trackNodes();
}

musicLineItem::~musicLineItem()
{
}

QRectF musicLineItem::boundingRect() const
{
	if (Pos == verticle)
	{
        return QRectF(0, -Length / 2, 0, Length);
	}
	if (Pos == horizontal)
	{
        return QRectF(-Length / 2, 0, Length, 0);
    }
}

void musicLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
    QWidget *widget)
{
    QPen pen(lineColor);
	painter->setPen(pen);
	if (Pos == verticle) 
	{
        painter->drawLine(0, -Length / 2, 0, Length / 2);
    }

	else if (Pos == horizontal)
	{
        painter->drawLine(-Length / 2, 0, Length / 2, 0);
	}
}


void musicLineItem::setColor(const QColor &color)
{
	lineColor = color;
	update();
}

/*
musicLineItem::~musicLineItem()
{
}
*/
    /*
    foreach (musicTextItem *item, parents)
    {
        item->removeLine(this);
    }
    */

/*
void musicLineItem::trackNodes()
{
    QPointF leftparent->pos
}
*/
