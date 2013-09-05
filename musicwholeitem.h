#ifndef MUSICWHOLEITEM_H
#define MUSICWHOLEITEM_H

#include <QGraphicsItem>
//#include <QList>

class musicRowItem;

class musicWholeItem
{
public:
    musicWholeItem();
    ~musicWholeItem();
    QRectF boundingRect() const;
    //void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    //QPainterPath shape() const;
    void addRow(musicRowItem *row);
    void setRowHeight(qreal height);
    void setPageWidth(qreal width);
    void setPos(qreal x, qreal y);
    QPointF pos();
    void arrangeRow();

private:
    QVector<musicRowItem *> lines;
    qreal rowHeight;
    qreal pageWidth;
    QPointF Pos;
    
};

#endif // MUSICWHOLEITEM_H
