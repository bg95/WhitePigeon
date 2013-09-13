#include "musicbaritem.h"
#include "musictextitem.h"
#include "musiclineitem.h"
#include <QtWidgets>

/*
musicBarItem::musicBarItem(QList<musicTextItem *> tune) 
    : numbers(tune), clapLength(1), textWidth(50)
{
	fillText();
    arrangeLines();
}
*/

musicBarItem::musicBarItem(QVector<musicTextItem *> tune)
    : numbers(tune), clapLength(1), textWidth(50), Pos(QPointF(0, 0))
{
    fillText();
    arrangeLines();
}

void musicBarItem::reset(QVector<musicTextItem *> tune)
{
    numbers = tune;
    fillText();

}
/*
void musicBarItem::paint(QPainter *painter,
                         const QStyleOptionGraphicsItem *option, QWidget *widget)
{
}
*/

QRectF musicBarItem::boundingRect() const
{
	musicTextItem *fir = numbers.first();
	musicTextItem *bac = numbers.back();
	QPointF firpos = fir->pos();
	qreal firwidth = fir->boundingRect().width();
	qreal firheight = fir->boundingRect().height();
	QPointF bacpos = bac->pos();
	qreal bacwidth = bac->boundingRect().width();
	//qreal bacheight = bac->boundingRect().height();
	qreal width = bacpos.x() - firpos.x() + firwidth / 2 + bacwidth / 2;
	qreal height = firheight;
	//QPointF leftup = QPointF(firpos.x() - firwidth / 2, firpos.y() - firlength / 2);
    return QRectF(-width / 2, -height / 2, width, height);
}



musicLineItem *musicBarItem::drawLine(musicTextItem *left, musicTextItem *right, int level)
{
    //for (int i = 0; i < level; ++i)
    //{
    //musicLineItem *thisline = new musicLineItem;
    qreal floor = left->pos().y() + left->boundingRect().height() / 2;
    qreal x1 = left->pos().x() - left->boundingRect().width() / 2;
    qreal x2 = right->pos().x() + right->boundingRect().width() / 2;
    qreal interval = left->interval();
    musicLineItem *thisline = new musicLineItem(x2 - x1, musicLineItem::horizontal);
    thisline->setPos((x1 + x2) / 2, floor + level * interval);
    lines.insert(thisline);
    return thisline;
    //}
}

void musicBarItem::setClap(const qreal length)
{
    clapLength = length;
    arrangeLines();
    //update();
}

void musicBarItem::removeLines()
{
    foreach(musicLineItem * line, lines)
    {
        delete line;
    }
    lines.clear();
}

/*
QPainterPath musicBarItem::shape() const
{
    QPainterPath painterpath;
    return painterpath;
}
*/

void musicBarItem::arrangeLines()
{
    //qDebug() << "haha";
    removeLines();
    foreach (musicTextItem *item, numbers) {
        for (int i = 0; i < item->lines(); ++i)
        {
            //musicLineItem *line = new musicLineItem;
            musicLineItem *newline = drawLine(item, item, i);
            item->pushLines(newline, i);
        }
    }
    QVector<qreal> partialsum(numbers.count() + 1);
    partialsum[0] = 0;
    qreal sum = 0;
    for (int i = 1; i < numbers.count() + 1; ++i)
    {
        sum += numbers[i - 1]->length();
        partialsum[i] = sum;
        //sum += numbers[i]->length();
    }
    //qDebug() << partialsum;
    qreal length = clapLength;
    int cnt = 1;
    for (qreal i = 1; i <= 4; ++i)
    {
        int prepos = -1;
        bool found = false;
        for (qreal j = 0; j <= sum; j += length)
        {
			int k = 0;
            for (k = k; k < partialsum.count(); ++k)
            {
                if (partialsum[k] == j)
                {
                    if (found)
                    {
                        qDebug() << "prepos: " << prepos << "now: " << k;
                        int l;
                        for (l = prepos; l < k; ++l)
                        {
                            qDebug() << "l:" << l << "k:" << k << "cnt" << cnt;
                            if (numbers[l]->lines() < cnt)
                            {
                                break;
                            }
                        }
                        if (l == k)
                        {
                            qDebug() << "haha";
                            //musicLineItem *newline =
                                    //drawLine(numbers[prepos], numbers[k-1], i);
                            for (int m = 1; m <= i; ++m)
                            {
                                if (numbers[prepos]->thisLine(m - 1) !=
                                        numbers[k - 1]->thisLine(m - 1))
                                {
                                	arrangeText(prepos, k);
                                    musicLineItem *newline =
                                            drawLine(numbers[prepos], numbers[k - 1], m - 1);
                                    for (l = prepos; l < k; ++l)
                                    {
                                        removeLine(numbers[l]->thisLine(m - 1));
                                        numbers[l]->pushLines(newline, m - 1);
                                    }
                                }
                            }
                            /*
                            for (int l = prepos; l < k; ++l)
                            {
                                for (int m = 1; m <= i; ++m)
                                {
                                    if
                                }
                                removeLine(numbers[l]->thisLine(i - 1));
                                numbers[l]->pushLines(newline, i - 1);
                            }
                            */
                        }
                    }
                    prepos = k;
                    found = true;
                    break;
                }
                else if (partialsum[k] > j)
                {
                	found = false;
                    break;
                }

            }

            /*
            if (qFind(partialsum.begin(), partialsum.end(), qreal(i)) != partialsum.end())
            {
                if (prefind)
                {

                }
            }
            else
            {
                prefind = false;
            }
            */
        }
        length /= 2;
        cnt++;
    }
}

void musicBarItem::removeLine(musicLineItem *line)
{
    delete line;
    lines.remove(line);
}

void musicBarItem::arrangeText(int begin, int end)
{
	int count = end - begin;
	if (count & 1 == 1) 
	{
		int middle = (end + begin) / 2;
		for (int i = begin; i < end; ++i) 
		{
			int distance = middle - i;
            QPointF oripos = numbers[i]->pos();
            numbers[i]->setPos(oripos.x() + distance * 8, oripos.y());
		}
	}
	else 
	{
		int middle = (end + begin) / 2;
		for (int i = begin; i < end; ++i)
		{
			int distance = middle - i;
            QPointF oripos = numbers[i]->pos();
            numbers[i]->setPos(oripos.x() + distance * 8 - 4, oripos.y());
		}
	}
    //update();
}
		
void musicBarItem::fillText() 
{
    qreal oriposx = pos().x();
    qreal oriposy = pos().y();
    //qDebug() << oriposx;
    //qDebug() << oriposy;
	int count = numbers.count();
    //qDebug() << count;
	for (int i = 0; i < count; ++i) 
	{
        numbers[i]->setPos(oriposx - count * textWidth / 2 + textWidth / 2+ i * textWidth, oriposy);
	}
}

void musicBarItem::setWidth(qreal width)
{
	textWidth = width;
}

/*
QVariant musicBarItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionHasChanged)
    {
        fillText();
        arrangeLines();
    }
    return QGraphicsItem::itemChange(change, value);
}
*/

QPointF musicBarItem::pos()
{
    return Pos;
}

void musicBarItem::setPos(qreal x, qreal y)
{
    Pos = QPointF(x, y);
    fillText();
    arrangeLines();

}

musicBarItem::~musicBarItem()
{
    foreach (musicLineItem *thisline, lines)
    {
        delete thisline;
    }
    lines.clear();
}

musicBarItem::addArcPairs(musicTextItem *front, musicTextItem *end);
{
  QPair<musicTextItem, musicTextItem> thispair(front, end);
  arcPairs.push_back(thispair);

}

void musicBarItem::arrangeArcs() {
  foreach (QPair<musicTextItem *, musicTextItem *> thispair, arcPairs) {
    qreal startx = arcPairs.first->pos().x() - 
      arcPairs.first->boundingRect().width / 2;
    qreal endx = arcPairs.second->pos().x() + 
      arcPairs.second->boundingRect().width / 2;
    musicArcItem *thisarcitem = new musicArcItem(endx - startx);
    int firstdots = arcPairs.first->upperDots.count();
    int seconddots = arcPairs.second->uppreDots.count();
    qreal firstheight = arcPairs.first->pos().y() - 
      arcPairs.first->boundingRect().height / 2 -
      firstdots * arcPairs.first->interval();
    qreal secondheight = arcPairs.second->pos().y() -
      arcPairs.second->boundingRect().height / 2 -
      seconddots * arcPairs.second->interval();
    qreal realheight = firstheight > secondheight ? secondheight : firstheight;
    
     


    














