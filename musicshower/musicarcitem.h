#ifndef MUSICARCITEM_H
#define MUSICARCITEM_H

#include <QGraphicsPathItem>

class musicArcItem : public QGraphicsPathItem
{
    //Q_OBJECT

public:
    musicArcItem(QPointF start, QPointF end, QGraphicsItem *parent = 0);

};

#endif // MUSICARCITEM_H
