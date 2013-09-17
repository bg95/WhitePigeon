#include "musicscene.h"
#include "musictextitem.h" 
#include "musicbaritem.h"
#include "musicbracketitem.h"
#include "WPScore/WPScore.h"
#include "musicrowitem.h"
#include "musicwholeitem.h"
#include "musicdotitem.h"
#include <QtWidgets>

const int musicScene::musicHeight[12] = {6, 13, 7, 1, 8, 2, 9, 3, 4, 11, 5, 12};

musicScene::musicScene(QWidget *parent)
  : QGraphicsScene(parent),
    partNumber(0),
    rowNumber(0),
    widget(NULL),   
    music(NULL)
{
    //setSceneRect(0, 0, 2000, 2000);
}

void musicScene::setScore(WPScore *score)
{
    music = score;
    display();
}

void musicScene::setRect(QRectF rect)
{
    setSceneRect(rect);
}

void musicScene::display() //unsave
{
  foreach (QVector<musicTextItem *> textvector, numbers) {
    foreach (musicTextItem *thistext, textvector) {
      delete thistext;
    }
    textvector.clear();
  }
  numbers.clear();
  foreach (QVector<musicBarItem *> barvector, bars) {
    foreach (musicBarItem *thisbar, barvector) {
      delete thisbar;
    }
    barvector.clear();
  }
  bars.clear();
  foreach (musicRowItem * thisrows, rows) {
    delete thisrows;
  }
  rows.clear();
    music->lockForRead();
    partNumber = music->countPartNumber();
    music->unlock();
    numbers.resize(partNumber);
    for (int i = 0; i < partNumber; ++i)
    {
      music->lockForRead();
      std::vector <WPMultinote> multiNote = music->getPartByOrder(i)->getAllNotes();
      music->unlock();
        std::vector<WPMultinote>::iterator iter;
        numbers[i].clear();
        for (iter = multiNote.begin(); iter != multiNote.end(); iter++)
        {
            WPNote thisNote = (iter->getNotes())[0];
            int height = thisNote.getPitch();
            musicTextItem *thisText;
            if (height == WPNote::Rest)
            {
                thisText = new musicTextItem('0');
            }
            else
            {
                int dots = 0;
                while (height >= 12)
                {
                    height -= 12;
                    ++ dots;
                }
                while (height < 0)
                {
                    height += 12;
                    -- dots;
                }
                dots += height > 2;
                int mheight = musicHeight[height];
                if (mheight > 7)
                {
                    thisText->setGoUp(true);
                    mheight -= 7;
                }
                thisText = new musicTextItem(char(mheight + '0'));
                if (dots > 0)
                {
                    thisText->addUpperDot(dots);
                }
                if (dots < 0)
                {
                    thisText->addLowerDot(- dots);
	        }
            }
            numbers[i].push_back(thisText);
            Fraction thisfrac = thisNote.getLength();
            //qDebug() << thisfrac.X << " " << thisfrac.Y;
            if (thisfrac > Fraction(1,4))
            {
                if ((thisfrac - Fraction(1, 4) * 1.5) < 0.01 && (thisfrac - Fraction(1, 4) * 1.5) > 0.01)
                {
                    thisText->setDotted(true);
                }
                int length = int(4 * (thisfrac.toDouble() + 0.01) - 1);
                for (int j = 0; j < length; j++)
                {
                    numbers[i].push_back(new musicTextItem('-'));
                }
            }
            int lines = 0;
            while (thisfrac < Fraction(1,4))
            {
                thisfrac = thisfrac + thisfrac;
                lines++;
            }
            if ((thisfrac - Fraction(1, 4) * 1.5) < 0.01 && (thisfrac - Fraction(1, 4) * 1.5) > -0.01) {
	      thisText->setDotted(true);
	    }
            thisText->setLines(lines);
        }
    }
    bars.resize(partNumber);
    for (int i = 0; i < partNumber; ++i)
    {
      qreal tmpsum = 0;
      int lastpos = 0;
      for (int j = 0; j < numbers[i].count(); ++j) {
        qreal tmplength = numbers[i][j]->length();
        //qDebug() << "tmplength " << tmplength;
        tmpsum += tmplength;
        if (tmpsum - 4 < 0.01 && tmpsum - 4 > -0.01) {
          QVector<musicTextItem *> textsegment;
          for (int k = lastpos; k <= j; ++k)
          {
              textsegment.push_back(numbers[i][k]);
          }
          //qDebug() << "create a " << j + 1 - lastpos << "bar";
          musicBarItem *thisbar = new musicBarItem(textsegment);
          //qDebug() << "textsegment " << textsegment.count();
          lastpos = j + 1;
          tmpsum = 0;
          bars[i].push_back(thisbar);
        }
      }
    }
    int barsNumber;
    //qDebug() << "parts" << bars.count();
    if (bars.count())
    {
      int maxbarnumber = bars[0].count();
      for (int i = 0; i < bars.count(); ++i) {
          //qDebug() << "bars[" << i << "]=" << bars[i].count();
    if (bars[i].count() > maxbarnumber) {
      maxbarnumber = bars[i].count();
	}
    barsNumber = maxbarnumber;
      }
    }
    else
    {
        barsNumber = 0;
    }
    rowNumber = barsNumber / 4 + (barsNumber % 4 != 0);
    rows.resize(rowNumber);

    for (int i = 0; i < rowNumber; ++i)
    {
        rows[i] = new musicRowItem(partNumber, 4);
    }
    for (int i = 0; i < partNumber; ++i)
    {
        for (int j = 0; j < barsNumber; ++j)
        {
            if (j < bars[i].count()) {
            rows[j / 4]->insertMusic(bars[i][j], i, j % 4);
            }
        }
    }
    widget = new musicWholeItem;
    for (int i = 0; i < rowNumber; ++i)
    {
        widget->addRow(rows[i]);
    }
    for (int i = 0; i < rowNumber; ++i)
    {
        foreach (musicLineItem *thisline, rows[i]->bars)
        {
            addItem((QGraphicsItem *)(thisline));
        }
        addItem((QGraphicsItem *)(rows[i]->bracket));
    }
    for (int i = 0; i < partNumber; ++i)
    {
        foreach (musicBarItem *thisbar, bars[i])
        {
            foreach (musicLineItem *thisline, thisbar->lines)
            {
                addItem((QGraphicsItem *)thisline);
            }
        }
    }
    for (int i = 0; i < partNumber; ++i)
    {
        foreach (musicTextItem *thistext, numbers[i])
        {
            addItem(thistext);
            foreach (musicDotItem *thisdot, thistext->upperDots)
            {
                //thisdot->setRadius(100);
                addItem((QGraphicsItem *)thisdot);
            }
            foreach (musicDotItem *thisdot, thistext->lowerDots)
            {
                //thisdot->setRadius(100);
                addItem((QGraphicsItem *)thisdot);
            }
        }
    }
    //qDebug() << "rownumber" << rowNumber;
    widget->arrangeRow();
    foreach (musicRowItem *thisline, rows) {
      thisline->arrangeBar();
      thisline->arrangeMusic();
    }
    for (int i = 0; i < bars.count(); ++i) {
      foreach (musicBarItem* thisbar, bars[i]) {
        thisbar->fillText();
        thisbar->arrangeLines();
        foreach (musicLineItem *thisline, thisbar->lines)
        {
            addItem((QGraphicsItem *)thisline);
        }
      }
    }
    /*
    for (int i = 0; i < numbers.count(); ++i) {
      foreach (musicTextItem *thistext, numbers[i]) {
        thistext->arrangeDots();
      }
    }
    */
}

musicScene::~musicScene()
{
    for (int i = 0; i < partNumber; ++i)
    {
        foreach (musicTextItem *thistext, numbers[i])
        {
            delete thistext;
        }
        numbers[i].clear();
    }
    numbers.clear();
    for (int i = 0; i < partNumber; ++i)
    {
        foreach (musicBarItem *thisbar, bars[i])
        {
            delete thisbar;
        }
        bars[i].clear();
    }
    bars.clear();
    for (int i = 0; i < rowNumber; ++i)
    {
        delete rows[i];
    }
    rows.clear();
    delete widget;
}

void musicScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
  musicTextItem *newtext = new musicTextItem(0);
  newtext->create(event);
  QGraphicsScene::mouseDoubleClickEvent(event);
}


