#include "musicbaritem.h"
#include "musictextitem.h"
#include "musiclineitem.h"
#include <QtWidgets>

musicBarItem::musicBarItem(QList<musicTextItem *> tune) 
    : numbers(tune), clapLength(1), textWidth(50)
{
	fillText();
    arrangeLines();
}

musicBarItem::reset(QList<musicTextItem *> tune)
{
    numbers = tune;
    fillText();
    arrangeLines();

    /*
    removeLines();
    QList<int> partialsum(numbers.count());

    for (int i = 0; i < numbers.count(); ++i)
    {

    }

    QQueue <musicTextItem *> continueline;
    qreal currentlength = 0;
    foreach (musicTextItem *number, numbers)
    {
        int numline = number->lines();
        if (numline == 0)
        {
            for (int i = 0; i < )
            continueline.clear();
            currentlength = 0;
        }
        else
        {
            if (number->length() >= 1)
            {
                continueline.clear();
                currentlength = 0;
                for (int i = 0; i < numline; ++i)
                {
                    drawline(number, number, i);
                }
            }
            else
            {
                continueline.enqueue(number);
                currentlength += number->length();
                if (currentlength > 1.0)
                {
                    for (int i = 0; i < continueline.count(); ++i)
                    {
                        musicTextItem *bede = continueline.dequeue();
                        currentlength -= bede->length();
                        if (currentlength < 1)
                        {
                            break;
                        }
                    }
                }
                else if (currentlength == 1)
                {

                }
            }
        }
    }
    */
}

void musicBarItem::paint(QPainter *painter,
                         const QStyleOptionGraphicsItem *option, QWidget *widget)
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

/*
void musicBarItem::setWidth(const qreal width)
{
    barWidth = width;
    update();
}

void musicBarItem::setHeight(const qreal height)
{
    barHeight = height;
    update();
}



void musicBarItem::addText(musicTextItem *number)
{
    numbers.push_back(number);
}
*/

musicLineItem *musicBarItem::drawLine(musicTextItem *left, musicTextItem *right, int level)
{
    //for (int i = 0; i < level; ++i)
    //{
    musicLineItem *thisline = new musicLineItem;
    qreal floor = left->pos().y() + left->boundingRect().height() / 2;
    qreal x1 = left->pos().x() - left->boundingRect().width() / 2;
    qreal x2 = right->pos().x() + right->boundingRect().width() / 2;
    qreal interval = left->interval();
    thisline->setLine(x1, floor + level * interval, x2, floor + level * interval);
    lines.insert(thisline);
    return thisline;
    //}
}

void musicBarItem::setClap(const qreal length)
{
    clapLength = length;
    arrangeLines();
    update();
}

void musicBarItem::removeLines()
{
    foreach(QSet<musicLineItem *> line, lines)
    {
        delete line;
    }
    lines.clear();
}

void musicBarItem::arrangeLines()
{
    removeLines();
    foreach (musicTextItem *item, numbers) {
        for (int i = 0; i < item->lines(); ++i)
        {
            //musicLineItem *line = new musicLineItem;
            musicLineItem *newline = drawLine(item, item, i);
            item->pushLines(newline, i);
        }
    }
    QList<qreal> partialsum(numbers.count() + 1);
    partialsum[0] = 0;
    qreal sum = 0;
    for (int i = 1; i < numbers.count() + 1; ++i)
    {
        sum += numbers[i - 1]->length();
        partialsum[i] = sum;
        //sum += numbers[i]->length();
    }
    qreal length = clapLength;
    for (qreal i = 1; i <= 4; ++i)
    {
        int prepos = -1;
        bool found = false;
        int cnt = 0;
        for (qreal j = 0; j <= sum; j += length)
        {
        	cnt++;
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
                    else
                    {
                        found = true;
                        prepos = k;
                    }
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
			QPointf oripos = numbers[i]->pos();
            numbers[i]->setPos(oripos.x() + distance * 4, oripos.y());
		}
	}
	else 
	{
		int middle = (end + begin) / 2;
		for (int i = begin; i < end; ++i)
		{
			int distance = middle - i;
			QPointf oripos = numbers[i]->pos();
            numbers[i]->setPos(oripos.x() + distance * 4 - 2, oripos.y());
		}
	}
	update();
}
		
void musicBarItem::fillText() 
{
	QPointF oriposx = pos().x();
	QPointF oriposy = pos().y();
	int count = numbers.count();
	for (int i = 0; i < count; ++i) 
	{
        numbers[i]->setPos(oriposx - count * textWidth / 2 + i * width, oriposy);
	}
}

void setWidth(qreal width)
{
	textWidth = width;
}

















