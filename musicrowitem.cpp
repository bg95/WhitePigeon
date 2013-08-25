#include "musicrowitem.h"
#include "musicbracketitem.h"
#include "musicbaritem.h"

musicRowItem::musicRowItem(int row, int collumn)
    : rowNumber(row), collumnNumber(collumn), rowWidth(50),
      collumnWidth(250), bars(row * collum), bracket(new musicBracketItem(row)),
{
    drawBar();
    arrangeBar();
    //arrangeMusic();
}

void musicRowItem::setBarDistance(qreal distance)
{
    collumnWidth = distance;
    update();
}

void musicRowItem::setRowDistance(qreal distance)
{
    rowWidth = distance;
    update();
}

QRectF musicRowItem::boundingRect() const
{
    qreal width = collumnWidth * collumnNumber + 10;
    qreal height = rowWidth * rowNumber + 10;
    return QRectF(-width / 2, -height / 2, width, height);
}

void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
}

QPainterPath shape() const
{
    QPainterPath painterpath;
    return painterpath;
}

void musicRowItem::drawBar()
{
    for (int i = 0; i < rowNumber; ++i)
    {
        for (int j = 0; j < collumnNumber; ++j)
        {
            bars[i * collumnNumber + j] = new musicLineItem;
        }
    }
}

void musicRowItem::arrangeBar()
{
    bracket->setPos(pos().x() - boundingRect().width() / 2 + 4, pos().y());
    qreal originx = bracket->pos().x() + 6;
    qreal originy = pos().y() - boundingRect().height() / 2 + 5;
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
        musics[(row - 1) * collumnNumber + (collumn - 1)] = barItem;
    }
    arrangeMusic();
}

void musicRowItem::arrangeMusic()
{
    qreal originx = pos().x() - boundingRect().width() / 2 + 10;
    qreal originy = pos().y() - boundingRect().height() / 2 + 5;
    for (int i = 0; i < rowNumber; ++i)
    {
        for (int j = 0; j < collumnNumber; ++j)
        {
            musics[i * collumnNumber + j]->setPos(originx + collumnWidth * (j + 0.5), originy + rowWidth * i);
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
















