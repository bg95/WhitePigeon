#ifndef MUSICBARITEM_H
#define MUSICBARITEM_H

#include <QGraphicsItem>
#include "musiclineitem.h"
#include <QVector>
#include <QSet>

class musicTextItem;
class musicLineItem;
class musicArcItem;
class Qset;

class musicBarItem
{
public:
  //void setText(const QString &Text);
    musicBarItem(QVector<musicTextItem *> tune);
    //musicBarItem(QList<musicTextItem *> tune);
    ~musicBarItem();
    //void reset(QVector <musicTextItem *> tune);
    QRectF boundingRect() const;
    void arrangeLines();
    void arrangeArcs();
    void addArcPairs(musicTextItem *front, musicTextItem *end);
    void arrangeText(int begin, int end);
    void setClap(const qreal length);
    void fillText();
    void setWidth(qreal width);
    void removePairs();
    //QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void setPos(qreal x, qreal y);
    QPointF pos();
    //void drawLine(musicTextItem *left, musicTextItem* right, int level);
    //void removeLines();
    QSet <musicLineItem *> lines;
    QVector <musicTextItem *> numbers;

private:
    //qreal barHeight;
    //qreal barWidth;
    qreal clapLength;
    qreal textWidth;
    //QVector <musicTextItem *> numbers;
    QSet <musicArcItem *> arcs;
    //QVector <musicArcItem *> arcs;
    QVector<QPair<musicTextItem *, musicTextItem *> > arcPairs;
    QPointF Pos;
    //QSet <musicLineItem *> lines;
    musicLineItem *drawLine(musicTextItem *left, musicTextItem* right, int level);
    void removeLines();
    void removeLine(musicLineItem *line);

};

#endif // MUSICBARITEM_H
