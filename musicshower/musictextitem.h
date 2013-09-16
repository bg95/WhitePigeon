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
<<<<<<< HEAD
    void setGoUp(bool go);
    void setGoDown(bool go);
    void setPos(qreal x, qreal y);
    void create(QGraphicsSceneMouseEvent *event);
=======
    //QVector <musicLineItem *> lines;
    //static qreal radius();
    //int lowerDots();
    //int lines();
    //int upperDots();
    //void addUpperDot(musicDotItem *upperDot);
>>>>>>> 02fefffa89294ac3b3f75f36a28e78d414bb2cc9
    QSet <musicDotItem *> upperDots;
    QSet <musicDotItem *> lowerDots;
    QChar musicHeight;

protected:
<<<<<<< HEAD
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
=======
    //void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
>>>>>>> 02fefffa89294ac3b3f75f36a28e78d414bb2cc9
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
<<<<<<< HEAD
    bool isGo;
    bool isBack;
=======
    //QSet <musicLineItem *> lines;
    //int lowerDots;
    //int lines;
    //int upperDots;
    //QVector <musicDotItem *> upperDots;
    //QVector <musicArcItem *> upperarc;
>>>>>>> 02fefffa89294ac3b3f75f36a28e78d414bb2cc9

};

#endif // MUSICTEXTITEM_H
