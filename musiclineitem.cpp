#include "musiclineitem.h"
#include "musictextitem.h"

#include <QtWidgets>

musicLineItem::musicLineItem()
{
    /*
    foreach (musicTextItem *item, parents)
    {
        item->addLine(this);
    }
    */
    //trackNodes();
}

void musicLineItem::setColor(const QColor &color)
{
    setPen(QPen(color));
}

musicLineItem::~musicLineItem()
{
    /*
    foreach (musicTextItem *item, parents)
    {
        item->removeLine(this);
    }
    */
}

/*
void musicLineItem::trackNodes()
{
    QPointF leftparent->pos
}
*/
