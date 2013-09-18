#include "musicbracketitem.h"
#include <QtWidgets>

musicBracketItem::musicBracketItem(int lines) 
	: lineNumber(lines), lineWidth(50)
{
}

QRectF musicBracketItem::boundingRect() const
{
	qreal height = lineNumber * lineWidth;
	return QRectF(-4, -height / 2 - 4, 8, height + 8);
}

void musicBracketItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                             QWidget *widget)
{
	qreal height = lineNumber * lineWidth;
    QPen pen;
    pen.setWidth(4);
    painter->setPen(pen);
    painter->drawLine(-2, -height / 2, -2, height / 2);
    pen.setWidth(1);
    painter->setPen(pen);
    painter->drawLine(2, -height / 2 - 2, 2, height / 2 + 2);
    painter->drawLine(0, -height / 2, 4, -height / 2 - 4);
    painter->drawLine(0, height / 2, 4, height / 2 + 4);
    //painter->drawLine(0, lineWidth * lineNumber, 3, lineWidth * lineNumber + 3);
}

musicBracketItem::~musicBracketItem()
{
}
