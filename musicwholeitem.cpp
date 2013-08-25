#include "musicwholeitem.h"
#include "musicrowitem.h"

musicWholeItem::musicWholeItem()
    : rowHeight(250) , pageWidth(1200)
{
}

void musicWholeItem::addRow(musicRowItem *row)
{
    lines.append(row);
    arrangeRow();
    update();
}

QRectF musicWholeItem::boundingRect() const
{
    qreal height = rowHeight * lines.count() + 400;
    return QRectF(-pageWidth / 2, -height / 2, pageWidth, height);
}

void musicWholeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

}

QPainterPath musicWholeItem::shape() const
{
    QPainterPath painterpath;
    return painterpath;
}

void musicWholeItem::setRowHeight(qreal height)
{
    rowHeight = height;
    update();
}

void musicWholeItem::setPageWidth(qreal width)
{
    pageWidth = width;
    update();
}

void musicWholeItem::arrangeRow()
{
    qreal xfloor = pos().y() - boundingRect().height() / 2 + 200;
    for (int i = 0; i < lines.count(); ++i)
    {
        lines[i]->setPos(pos().x(), xfloor + i * rowHeight);
    }
}

musicWholeItem::~musicWholeItem()
{

}
