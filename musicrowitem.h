#ifndef MUSICROWITEM_H
#define MUSICROWITEM_H

#include <QGraphicsItem>
#include <QList>

class musicLineItem;
class musicBracketItem;
class musicBarItem;

class musicRowItem : public QGraphicsItem
{
public:
    musicRowItem(int row, int collumn);
    ~musicRowItem();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;
    void setBarDistance(qreal distance);
    void setRowDistance(qreal distance);
    void drawBar();
    void arrangeBar();
    void insertMusic(musicBarItem *barItem, int row, int collumn);
    void arrangeMusic();

private:
    int rowNumber;
    int collumnNumber;
    qreal collumnWidth;
    qreal rowWidth;
    QVector<musicLineItem *> bars;
    musicBracketItem *bracket;
    QVector<musicBarItem *> musics;
};

#endif // MUSICROWITEM_H
