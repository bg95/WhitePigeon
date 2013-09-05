#ifndef MUSICBRACKETITEM_H
#define MUSICBRACKETITEM_H

#include <QGraphicsItem>

class musicBracketItem : public QGraphicsItem
{
public:
    musicBracketItem(int lines);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

private:
	int lineNumber;
	qreal lineWidth;
    
};

#endif // MUSICBRACKETITEM_H
