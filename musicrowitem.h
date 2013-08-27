#ifndef MUSICROWITEM_H
#define MUSICROWITEM_H

#include <QGraphicsItem>

class musicLineItem;
class musicBracketItem;
class musicBarItem;


class musicRowItem
{
public:
    musicRowItem(int row, int collumn);
    ~musicRowItem();
    QRectF boundingRect() const;
    //void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    //QPainterPath shape() const;
    void setBarDistance(qreal distance);
    void setRowDistance(qreal distance);
    void drawBar();
    void arrangeBar();
    void insertMusic(musicBarItem *barItem, int row, int collumn);
    void arrangeMusic();
    //QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    QVector<musicLineItem *> bars;
    musicBracketItem *bracket;
    QPointF pos();
    void setPos(qreal x, qreal y);

private:
    int rowNumber;
    int collumnNumber;
    qreal collumnWidth;
    qreal rowWidth;
    //QVector<musicLineItem *> bars;
    //musicBracketItem *bracket;
    QVector<musicBarItem *> musics;
    QPointF Pos;
};

#endif // MUSICROWITEM_H
