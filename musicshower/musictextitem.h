#ifndef MUSICTEXTITEM_H
#define MUSICTEXTITEM_H

#include <QGraphicsItem>
#include <QSet>
#include <QFont>

class musicDotItem;
class musicLineItem;


class musicTextItem : public QGraphicsItem
{
public:
    musicTextItem(const QChar &number);
    ~musicTextItem();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setFont(const QFont &font);
    void setColor(const QColor &color);
    void setInterval(const qreal);
    void setLines(const int line);
    void addUpperDot(const int number);
    void addLowerDot(const int number);
    //void addLine(musicLineItem *line);
    void removeUpperDot();
    void removeLowerDot();
    void pushLines(musicLineItem *item, int level);
    musicLineItem *thisLine(int level);
    //void removeLine(musicLineItem *line);
    void arrangeDots();
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    int lines();
    qreal length();
    qreal interval();
    void setDotted(bool dot);
    void setGoUp(bool go);
    void setGoDown(bool go);
    void setPos(qreal x, qreal y);
    void create(QGraphicsSceneMouseEvent *event);
    QSet <musicDotItem *> upperDots;
    QSet <musicDotItem *> lowerDots;
    QChar musicHeight;

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    //void focusOutEvent(QFocusEvent *event);

private:
    //QChar musicHeight;
    QFont numberFont;
    QColor numberColor;
    qreal Interval;
    bool dotted;
    //QSet <musicDotItem *> upperDots;
    //QSet <musicDotItem *> lowerDots;
    QVector <musicLineItem *> Lines;
    //int numberLines;
    qreal calculate(int x);
    bool isGo;
    bool isBack;

};

#endif // MUSICTEXTITEM_H
