#include "musicbaritem.h"
#include "musictextitem.h"
#include "musiclineitem.h"
#include "musicarcitem.h"
#include <QtWidgets>

musicBarItem::musicBarItem(QVector<musicTextItem *> tune)
    : numbers(tune), clapLength(1), textWidth(30), Pos(QPointF(0, 0))
{
}

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
    qreal floor1 = left->pos().y() + left->boundingRect().height() / 2;
    qreal floor2 = right->pos().y() + right->boundingRect().height() / 2;
    qreal floor = (floor2 > floor1 ? floor2 : floor1);
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
                        int l;
                        for (l = prepos; l < k; ++l)
                        {
                            if (numbers[l]->lines() < cnt)
                            {
                                break;
                            }
                        }
                        if (l == k)
                        {
                            for (int m = 1; m <= i; ++m)
                            {
                                if (numbers[prepos]->thisLine(m - 1) !=
                                        numbers[k - 1]->thisLine(m - 1))
                                {
                                    musicLineItem *newline =
                                            drawLine(numbers[prepos], numbers[k - 1], m - 1);
                                    for (l = prepos; l < k; ++l)
                                    {
                                        removeLine(numbers[l]->thisLine(m - 1));
                                        numbers[l]->pushLines(newline, m - 1);
                                    }
                                }
                            }
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
    int count = numbers.count();
    while (count * textWidth > 250)
    {
        textWidth /= 2;
    }
    for (int i = 0; i < count; ++i) 
    {
      numbers[i]->setPos(oriposx - count * textWidth / 2 + textWidth / 2+ i * textWidth, oriposy);
    }
}

void musicBarItem::setWidth(qreal width)
{
    textWidth = width;
}

QPointF musicBarItem::pos()
{
    return Pos;
}

void musicBarItem::setPos(qreal x, qreal y)
{
    Pos = QPointF(x, y);
}

musicBarItem::~musicBarItem()
{
    foreach (musicLineItem *thisline, lines)
    {
        delete thisline;
    }
    lines.clear();
    foreach (musicArcItem *thisArc, arcs) {
      delete thisArc;
    }
    arcs.clear();
    for (int i = 0; i < arcPairs.count(); ++i)
    {
        QPair<musicTextItem *, musicTextItem *> thispair = arcPairs[i];
        delete thispair.first;
        delete thispair.second;
    }
    arcPairs.clear();
}

void musicBarItem::addArcPairs(musicTextItem *front, musicTextItem *end)
{
  QPair<musicTextItem *, musicTextItem *> thispair(front, end);
  arcPairs.push_back(thispair); 
}

void musicBarItem::removePairs() {
  foreach (musicArcItem *thisArc, arcs) {
    delete thisArc;
  }
  arcs.clear();
  for (int i = 0; i < arcPairs.count(); ++i)
  {
      QPair<musicTextItem *, musicTextItem *> thispair = arcPairs[i];
      delete thispair.first;
      delete thispair.second;
  }
  arcPairs.clear();
}

void musicBarItem::arrangeArcs() {
  foreach (musicArcItem *thisarcitem, arcs) {
    delete thisarcitem;
  }
  arcs.clear();
  for (int i = 0; i < arcPairs.count(); ++i) {
      QPair<musicTextItem *, musicTextItem *> thispair = arcPairs[i];
    qreal startx = thispair.first->pos().x();
    qreal endx = thispair.second->pos().x();
    qreal lengthx = endx - startx;
    musicArcItem *thisarcitem = new musicArcItem(lengthx);
    arcs.insert(thisarcitem);
    int firstdots = thispair.first->upperDots.count();
    int seconddots = thispair.second->upperDots.count();
    qreal firstheight = thispair.first->pos().y() -
            (thispair.first)->boundingRect().height() / 2 -
      firstdots * thispair.first->interval();
    qreal secondheight = thispair.second->pos().y() -
            (thispair.second)->boundingRect().height() / 2 -
      seconddots * thispair.second->interval();
    qreal realheight = firstheight > secondheight ? secondheight : firstheight;
    qreal xpos = (startx + endx) / 2;
    qreal ypos = realheight - lengthx / 8;
    thisarcitem->setPos(xpos, ypos);
  }
}
    
     


    














