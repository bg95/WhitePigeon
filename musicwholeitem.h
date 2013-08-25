#ifndef MUSICWHOLEITEM_H
#define MUSICWHOLEITEM_H

#include <QGraphicsItem>
#include <QList>

class musicRowItem;

class musicWholeItem : public QGraphicsItem
{
public:
    musicWholeItem();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;
    void addRow(musicRowItem *row);
    void setRowHeight(qreal height);
    void setPageWidth(qreal width);
    void arrangeRow();

private:
    QList<musicRowItem *> lines;
    qreal rowHeight;
    qreal pageWidth;
    
    
};

#endif // MUSICWHOLEITEM_H
