#include "musicscene.h"
#include "musictextitem.h"
#include "musicbaritem.h"
#include "musicbracketitem.h"
#include "WPScore/WPScore.h"
#include "musicrowitem.h"
#include "musicwholeitem.h"
#include <QtWidgets>

const int musicScene::musicHeight[12] = {6, -1, 7, 1, -1, 2, -1, 3, 4, -1, 5, -1};

musicScene::musicScene(QWidget *parent)
    : QGraphicsScene(parent),
      partNumber(0),
      rowNumber(0),
      widget(NULL),
      music(NULL)
{
    setSceneRect(0, 0, 2000, 2000);
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

void musicScene::display()
{
    partNumber = music->countPartNumber();
    numbers.resize(partNumber);
    for (int i = 0; i < partNumber; ++i)
    {
        std::vector <WPMultinote> multiNote = music->getPartByOrder(i)->getAllNotes();
        std::vector<WPMultinote>::iterator iter;
        numbers[i].clear();
        for (iter = multiNote.begin(); iter != multiNote.end(); iter++)
        {
            WPNote thisNote = (iter->getNotes())[0];
            int height = thisNote.getPitch();
            musicTextItem *thisText;
            if (height == WPNote::Rest)
            {
                thisText = new musicTextItem(0);
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
                thisText = new musicTextItem(
                            char(musicHeight[height] + '0'));
                if (dots > 0)
                {
                    thisText->addUpperDot(dots);
                }
                if (dots < 0)
                {
                    thisText->addLowerDot(dots);
                }
            }
            numbers[i].push_back(thisText);
            Fraction thisfrac = thisNote.getLength();
            if (thisfrac > Fraction(1,1))
            {
                int length = int(thisfrac.toDouble() - 1);
                for (int j = 0; j < length; ++j)
                {
                    numbers[i].push_back(new musicTextItem('-'));
                }
            }
            int lines = 0;
            while (thisfrac < Fraction(1,1))
            {
                thisfrac = thisfrac + thisfrac;
                lines++;
            }
            thisText->setLines(lines);
        }
    }
    bars.resize(partNumber);
    for (int i = 0; i < partNumber; ++i)
    {
        musicBarItem *thisbar = new musicBarItem(numbers[i]);
        bars[i].push_back(thisbar);
    }

    int barsNumber;
    if (bars.size())
    {
        barsNumber = bars[0].count();
    }
    else
    {
        barsNumber = 0;
    }

    rowNumber = barsNumber / 4 + (barsNumber % 4 != 0);
    rows.resize(rowNumber);

    for (int i = 0; i < rowNumber; ++i)
    {
        //qDebug() << "Hi";
        rows[i] = new musicRowItem(partNumber, 4);
        //qDebug() << partNumber;
        //qDebug() << "Hie";
    }

    for (int i = 0; i < partNumber; ++i)
    {
        for (int j = 0; j < barsNumber; ++j)
        {
            //qDebug("%d %d %d\n", rows.size(), i, j);
            rows[j / 4]->insertMusic(bars[i][j], i + 1, j % 4 + 1);
        }
    }

    widget = new musicWholeItem;
    widget->setPos(sceneRect().width() / 2, sceneRect().height() / 2);
    for (int i = 0; i < rowNumber; ++i)
    {
        widget->addRow(rows[i]);
    }
    //qDebug() << widget->pos().x() << " " << widget->pos().y();
    //addItem(widget);
    //qDebug() << rowNumber;
    //numbers[0][0]->setPos(900, 5000);
    for (int i = 0; i < rowNumber; ++i)
    {
        //qDebug() << rows[i]->pos().x() << " " << rows[i]->pos().y();
        //qDebug() << (rows[i]->bracket)->pos();
        //qDebug() << (rows[i]->bracket)->boundingRect();
        //addItem(rows[i]);
        //qDebug() << rows[i]->bars.count();
        qDebug() << rows[i]->pos();
        foreach (musicLineItem *thisline, rows[i]->bars)
        {
            //qDebug() << thisline->pos();
            addItem((QGraphicsItem *)(thisline));
        }
        addItem((QGraphicsItem *)(rows[i]->bracket));
    }
    for (int i = 0; i < partNumber; ++i)
    {
        foreach (musicBarItem *thisbar, bars[i])
        {
            //addItem(thisbar);
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
                addItem((musicTextItem *)thisdot);
            }
            foreach (musicDotItem *thisdot, thistext->lowerDots)
            {
                addItem((musicTextItem *)thisdot);
            }
        }
    }
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























