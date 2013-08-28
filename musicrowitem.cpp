#include "musicrowitem.h"
#include "musicbracketitem.h"
#include "musicbaritem.h"
#include <QtWidgets>

musicRowItem::musicRowItem(int row, int collumn)
    : rowNumber(row), collumnNumber(collumn), rowWidth(50),
      collumnWidth(250), bracket(new musicBracketItem(row)),
      Pos(QPointF(0, 0)), barHeight(30)
{
    bars.resize(row * collumn);
    drawBar();
    arrangeBar();
    musics.resize(row * collumn);
    //arrangeMusic();
}

void musicRowItem::setBarDistance(qreal distance)
{
    collumnWidth = distance;
    //update();
}

void musicRowItem::setRowDistance(qreal distance)
{
    rowWidth = distance;
    //update();
}

QRectF musicRowItem::boundingRect() const
{
    qreal width = collumnWidth * collumnNumber + 14;
    qreal height = rowWidth * rowNumber + 10;
    return QRectF(-collumnWidth * collumnNumber / 2 - 14, -height / 2, width, height);
}

/*
void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
}


QPainterPath musicRowItem::shape() const
{
    QPainterPath painterpath;
    return painterpath;
}
*/


void musicRowItem::drawBar()
{
    //qreal height = musics[0]->boundingRect().height() + 5;
    for (int i = 0; i < rowNumber; ++i)
    {
        for (int j = 0; j < collumnNumber; ++j)
        {
            musicLineItem* thisLine = new musicLineItem(barHeight, musicLineItem::verticle);
            bars[i * collumnNumber + j] = thisLine;
        }
    }
}


void musicRowItem::arrangeBar()
{
    bracket->setPos(pos().x() - collumnNumber * collumnWidth / 2 - 10, pos().y());
	  
    qreal originx = pos().x() - collumnNumber * collumnWidth / 2;
    qreal originy = pos().y() - rowWidth * rowNumber / 2 + rowWidth / 2;
    for (int i = 0; i < rowNumber; ++i)
    {
        for (int j = 0; j < collumnNumber; ++j)
        {
            bars[i * collumnNumber + j]->setPos(originx + collumnWidth * (j + 1), originy + rowWidth * i); 
        }
    }
}


void musicRowItem::insertMusic(musicBarItem *barItem, int row, int collumn)
{
    if (row <= rowNumber && collumn <= collumnNumber)
    {
        //qDebug("Here! %d %d", (row - 1) * collumnNumber + (collumn - 1), musics.size());
        musics[(row - 1) * collumnNumber + (collumn - 1)] = barItem;
    }
    //qDebug("There!");
    arrangeMusic();
    //qDebug("Over!");
}


void musicRowItem::arrangeMusic()
{
    qreal originx = pos().x() - collumnWidth * collumnNumber / 2;
    qreal originy = pos().y() - rowNumber * rowWidth / 2 + rowWidth / 2;
    for (int i = 0; i < rowNumber; ++i)
    {
        for (int j = 0; j < collumnNumber; ++j)
        {
            //qDebug("Wu %d %d", i, j);
            if (musics[i * collumnNumber + j])
            {
                musics[i * collumnNumber + j]->setPos(originx + collumnWidth * (j + 0.5), originy + rowWidth * i);
                //qDebug() << musics[i * collumnNumber + j]->pos();
            }
            //qDebug("Zhelun");
        }
    }
}


musicRowItem::~musicRowItem()
{
    foreach (musicLineItem *thisbar, bars)
    {
        delete thisbar;
    }
    bars.clear();
}


/*
QVariant musicRowItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionHasChanged)
    {
        arrangeBar();
        arrangeMusic();
    }
    return QGraphicsItem::itemChange(change, value);
}
*/

void musicRowItem::setPos(qreal x, qreal y)
{
    Pos = QPointF(x, y);
    arrangeBar();
    arrangeMusic();
}

QPointF musicRowItem::pos()
{
    return Pos;
}

void musicRowItem::setBarHeight(qreal height)
{
    barHeight = height;
}










