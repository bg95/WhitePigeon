#include "musicdotitem.h"
#include "musictextitem.h"

#include <QtWidgets>

musicDotItem::musicDotItem()
    : dotColor(Qt::black), dotRadius(1)
{
    /*
    if (dotPos == upper)
    {
        parentNumber->addUpperDot(this);
    }
    else
    {
        parentNumber->addLowerDot(this);
    }
    //track();
    */
}

musicDotItem::~musicDotItem()
{
    /*
    if (dotPos == upper)
    {
        parentNumber->removeUpperDot(this);
    }
    else
    {
        parentNumber->removeLowerDot(this);
    }
    */
}

void musicDotItem::setColor(const QColor &color)
{
    dotColor = color;
    update();
}

void musicDotItem::setRadius(const qreal radius)
{
    dotRadius = radius;
    update();
}

QRectF musicDotItem::boundingRect() const
{
    return QRectF(-dotRadius, -dotRadius, 2 * dotRadius, 2 * dotRadius);
}

void musicDotItem::paint(QPainter *painter,
                         const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    painter->setBrush(QBrush(dotColor));
    QRectF rect = boundingRect();
    painter->drawEllipse(rect);
}

/*
void musicDotItem::track()
{
    if (dotPos == upper)
    {
        int upperDots = parentNumber->upperDots();
        int height = parentNumber
    }

}
*/

qreal musicDotItem::radius()
{
    return dotRadius;
}




