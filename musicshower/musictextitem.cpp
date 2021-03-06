#include "musictextitem.h"
#include "musicdotitem.h"
#include "musiclineitem.h"
#include <QtWidgets>


musicTextItem::musicTextItem(const QChar &number)
    : musicHeight(number), numberFont("Times", 20, QFont::Bold),
      numberColor(Qt::black), Interval(4), dotted(false), isGo(false),
      isBack(false), uDot(0), lDot(0)
{
  setFlags(ItemIsMovable);
}

void musicTextItem::setFont(const QFont &font)
{
  prepareGeometryChange();
    numberFont = font;
    update();
}

void musicTextItem::setColor(const QColor &color)
{
    numberColor = color;
    update();
}

QRectF musicTextItem::boundingRect() const
{
    QFontMetricsF metrics(numberFont);
    QRectF rect = metrics.boundingRect(musicHeight);
    if (dotted)
    {
        return QRectF(-3 * rect.width() / 4, -rect.height() / 2,
                     3 * rect.width() / 2, rect.height());
    }
    return QRectF(-rect.width() / 2, -rect.height() / 2,
                 rect.width(), rect.height());
}

void musicTextItem::paint(QPainter *painter,
                          const QStyleOptionGraphicsItem */*option*/, 
			  QWidget */*widget*/)
{
    qreal length = boundingRect().width();
    qreal height = boundingRect().height();
    QRectF rect = QRectF(-length / 2, (-height - 10) / 2, length, height + 10);
    if (dotted)
    {
        painter->setBrush(Qt::black);
        QRectF dotrect = QRectF(rect.width() / 2 - 1, -1, 2, 2);
        painter->drawEllipse(dotrect);
    }
    painter->setFont(numberFont);
    painter->setPen(numberColor);
    //painter->setFont(QFont("Times", 15));
    if (isGo)
    {
        QRectF newRect(-length / 2 - 10, (-height - 10) / 2 - 10, 20, 20);
        painter->drawText(newRect, Qt::AlignCenter, QObject::tr("#"));
    }
    if (isBack)
    {
        QRectF newRect(-length / 2 - 10, (-height - 10) / 2 - 10, 10, 10);
        painter->drawText(newRect, Qt::AlignCenter, QObject::tr("b"));
    }
    painter->setFont(numberFont);
    painter->drawText(rect, Qt::AlignCenter, musicHeight);
    painter->setBrush(Qt::black);
    int numberLines = Lines.count();
    qreal inter = - Interval / 2;
    for (int i = 0; i < uDot; ++i)
    {
        QRectF dotrect(-1, -1 - (boundingRect().height() + 12)/ 2 - inter, 2, 2);
        //QRectF dotrect(center.x() - 1, center.y() - 1, 2, 2);
        painter->drawEllipse(dotrect);
        inter += Interval;
        //qDebug() << "print a upperdot at " << center.y();
        //qDebug() << " but the text is at " << pos().y();
    }
    inter = Interval * (numberLines - 1) ;
    for (int i = 0; i < lDot; ++i)
    {
        QRectF dotrect(-1, -1 + (boundingRect().height() + 10)/ 2 + inter, 2, 2);
        //QRectF dotrect(center.x() - 1, center.y() - 1, 2, 2);
        painter->drawEllipse(dotrect);
        inter += Interval;
        //qDebug() << "print a lowerdot at " << center.y();
        //qDebug() << " but the text is at " << pos().y();
    }
}

void musicTextItem::addUpperDot(const int number)
{
    prepareGeometryChange();
    uDot = number;
    update();
}

void musicTextItem::addLowerDot(const int number)
{
    prepareGeometryChange();
    lDot = number;
    update();
}

void musicTextItem::removeUpperDot()
{
    foreach (musicDotItem *item, upperDots)
    {
        delete item;
    }
    upperDots.clear();
}


void musicTextItem::removeLowerDot()
{
    foreach (musicDotItem *item, lowerDots)
    {
        delete item;
    }
    lowerDots.clear();
}

void musicTextItem::arrangeDots()
{
    int numberLines = Lines.count();
    qreal inter = - Interval / 2;
    foreach (musicDotItem *thisDot, upperDots)
    {
        thisDot->setPos(pos().x(), pos().y() - (boundingRect().height() + 12)/ 2 - inter);
        inter += Interval;
    }
    inter = Interval * (2 * numberLines - 1) / 2;
    foreach (musicDotItem *thisDot, lowerDots)
    {
        thisDot->setPos(pos().x(), pos().y() + (boundingRect().height() + 10)/ 2 + inter);
        inter += Interval;
    }
}


musicTextItem::~musicTextItem()
{
    foreach (musicDotItem *thisDot, upperDots) {
        delete thisDot;
    }
    foreach (musicDotItem *thisDot, lowerDots) {
        delete thisDot;
    }
}

void musicTextItem::setInterval(const qreal inter)
{
    Interval = inter;
}

void musicTextItem::setPos(qreal x, qreal y)
{
    QGraphicsItem::setPos(x, y);
}

void musicTextItem::setLines(const int line)
{
    Lines.resize(line);
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
    return Lines.count();
}

qreal musicTextItem::length()
{
    int numberLines = Lines.count();
    if (musicHeight == QChar('-'))
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
    return Interval;
}

void musicTextItem::pushLines(musicLineItem *line, int level)
{
    Lines[level] = line;
}

musicLineItem *musicTextItem::thisLine(int level)
{
    return Lines[level];
}

void musicTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
  QString text = QInputDialog::getText(event->widget(),
                       QObject::tr("Edit music height"),
                       QObject::tr("Enter new music height:"),
				       QLineEdit::Normal, QString(musicHeight));
  if (text.size() == 1) {
    musicHeight = text.at(0);
  }
  /*
  int retadd = QMessageBox::question(this, QObject::tr())
  */
}

void musicTextItem::create(QGraphicsSceneMouseEvent *event) {
    setPos(event->scenePos().x(), event->scenePos().y());
  QString text = QInputDialog::getText(event->widget(),
                       QObject::tr("Edit music height"),
                       QObject::tr("Enter new music height:"),
				       QLineEdit::Normal, QString(musicHeight));
  if (text.size() == 1) {
    musicHeight = text.at(0);
  }
}

void musicTextItem::setGoUp(bool go)
{
    isGo = go;
}

void musicTextItem::setGoDown(bool go)
{
    isBack = go;
}

