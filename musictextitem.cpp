#include "musictextitem.h"
#include "musicdotitem.h"
#include "musiclineitem.h"

#include <QtWidgets>

musicTextItem::musicTextItem(const QChar &number)
    : musicHeight(number), numberFont("Times", 20, QFont::Bold),
      numberColor(Qt::black), interval(4), dotted(false), numberLines(0)
{
    //setFlag(QGraphicsItem::ItemIsSelectable)
}

void musicTextItem::setFont(const QFont &font)
{
    numberFont = font;
    update();
}

void musicTextItem::setColor(const QColor &color)
{
    numberColor = color;
    update();
}

/*
void musicTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    //qDebug() << "why not";
    if (textInteractionFlags() == Qt::NoTextInteraction)
    {
        setTextInteractionFlags(Qt::TextEditable);
    }
    QGraphicsTextItem::mouseDoubleClickEvent(event);
}

void musicTextItem::focusOutEvent(QFocusEvent *event)
{
    setTextInteractionFlags(Qt::NoTextInteraction);
    QGraphicsTextItem::focusOutEvent(event);
}
*/

QRectF musicTextItem::boundingRect() const
{

    QFontMetricsF metrics(numberFont);
    QRectF rect = metrics.boundingRect(musicHeight);
    //rect.moveCenter(QPointF(rect.width() / 2, rect.height() / 2));
    if (dotted)
    {
        return QRectF(-3 * rect.width() / 4, -rect.height() / 2,
                     3 * rect.width() / 2, rect.height());
    }
    return QRectF(-rect.width() / 2, -rect.height() / 2,
                 rect.width(), rect.height());
    //return (1, 1, 1, 1);
}

void musicTextItem::paint(QPainter *painter,
                          const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    painter->setFont(numberFont);
    painter->setPen(numberColor);
    QRectF rect = boundingRect();
    if (dotted)
    {
        rect = QRectF(-rect.width() / 2, -rect.height() / 2,
                      2 * rect.width() / 3, rect.height());
        painter->drawText(rect, musicHeight);
        QPen pen;
        pen.setWidth(2);
        painter->setPen(pen);
        painter->drawPoint(3 * rect.width() / 4, rect.height() / 4);
    }
    painter->drawText(rect, musicHeight);
}

void musicTextItem::addUpperDot(const int number)
{
    for (int i = 0; i < number; ++i)
    {
        upperDots.insert(new musicDotItem);
    }
    //upperDots.insert(new musicDotItem);
    arrangeDots();
}

void musicTextItem::addLowerDot(const int number)
{
    for (int i = 0; i < number; ++i)
    {
        lowerDots.insert(new musicDotItem);
    }
    //lowerDots.insert(new musicDotItem);
    arrangeDots();
    //lowerDots++;
}

/*

void musicTextItem::addLine(musicLineItem *line)
{
    lines.insert(line);
    //lines++;
}

*/

void musicTextItem::removeUpperDot()
{
    foreach (musicDotItem *item, upperDots)
    {
        delete item;
    }
    upperDots.clear();

    /*
    QSet <musicDotItem *>::iterater iter = upperDots.begin();
    for (int i = 0; i < number; ++i)
    {
        delete *iter;
        iter = upperDots.erase(iter);
    }
    */
    //upperDots.remove(dot);
    //arrangeDots();
    //upperDots--;
}

void musicTextItem::removeLowerDot()
{
    foreach (musicDotItem *item, lowerDots)
    {
        delete item;
    }
    lowerDots.clear();

    /*
    QSet <musicDotItem *>::iterater iter = lowerDots.begin();
    for (int i = 0; i < number; ++i)
    {
        delete *iter;
        iter = lowerDots.erase(iter);
    }
    */
    //lowerDots.remove(dot);
    //arrangeDots();
    //lowerDots--;
}

/*

void musicTextItem::removeLine(musicLineItem *line)
{
    lines.remove(line);
    //lines--;
}

*/

void musicTextItem::arrangeDots()
{
    numberLines = lines.count();
    qreal inter = - interval / 2;
    foreach (musicDotItem *thisDot, upperDots)
    {
        thisDot->setPos(pos().x(), pos().y() - boundingRect().height() / 2 - inter);
        inter += interval;
    }
    inter = interval * (2 * numberLines - 1) / 2;
    foreach (musicDotItem *thisDot, lowerDots)
    {
        thisDot->setPos(pos().x(), pos().y() + boundingRect().height() / 2 + inter);
        inter += interval;
    }
}

/*

int musicTextItem::upperDots()
{
    return upperDots;
}

int musicTextItem::lowerDots()
{
    return lowerDots;
}

int musicTextItem::lines()
{
    return lines;
}

*/

musicTextItem::~musicTextItem()
{
    /*
    foreach (musicLineItem *thisLine, lines) {
        delete thisLine;
    }
    */
    foreach (musicDotItem *thisDot, upperDots) {
        delete thisDot;
    }
    foreach (musicDotItem *thisDot, lowerDots) {
        delete thisDot;
    }
}

void musicTextItem::setInterval(const qreal inter)
{
    interval = inter;
    arrangeDots();
}

void musicTextItem::setLines(const int line)
{
    //numberLines = line;
    lines.resize(line);
    arrangeDots();
}

QVariant musicTextItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionHasChanged)
    {
        arrangeDots();
    }
    return QGraphicsItem::itemChange(change, value);
}

int musicTextItem::lines()
{
    return lines.count();
}

qreal musicTextItem::length()
{
    if (musicHeight == QChar('-'));
    {
        return 1;
    }
    if (dotted)
    {
        return calculate(numberLines) * (1 + 0.5);
    }
    return calculate(numberLines);
}

void musicTextItem::setDotted(bool dot)
{
    dotted = dot;
    update();
}

qreal musicTextItem::calculate(int x)
{
    qreal answer = 1;
    for (int i = 0; i < x; ++i)
    {
        answer /= 2;
    }
    return answer;
}

qreal musicTextItem::interval()
{
    return interval;
}

void musicTextItem::pushLines(musicLineItem *line, int level)
{
    lines[level] = line;
}

musicTextItem *thisLine(int level)
{
    return lines[level];
}






