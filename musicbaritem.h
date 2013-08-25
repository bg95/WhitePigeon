#ifndef MUSICBARITEM_H
#define MUSICBARITEM_H

#include <QGraphicsItem>
#include <QList>

class musicTextItem;
class musicLineItem;

class musicBarItem : public QGraphicsItem
{

public:
    musicBarItem(QList <musicTextItem *> tune);
    ~musicBarItem();
    void reset(QList <musicTextItem *> tune);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;
    //void addText(musicTextItem *number);
    //void removeText(musicTextItem *number);
    //void setHeight(const qreal height);
    //void setWidth(const qreal width);
    void arrangeLines();
    void arrangText();
    void setClap(const qreal length);
    void fillText();
    void setWidth();
    //void drawLine(musicTextItem *left, musicTextItem* right, int level);
    //void removeLines();

private:
    //qreal barHeight;
    //qreal barWidth;
    qreal clapLength;
    qreal textWidth;
    QList <musicTextItem *> numbers;
    QSet <musicLineItem *> lines;
    musicLineItem *drawLine(musicTextItem *left, musicTextItem* right, int level);
    void removeLines();
    void removeLine(musicLineItem *line);

};

#endif // MUSICBARITEM_H
