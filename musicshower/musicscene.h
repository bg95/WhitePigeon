#ifndef MUSICSCENE_H
#define MUSICSCENE_H

#include <QGraphicsScene>
#include <QVector>
#include "WPScore/WPScore.h"

//class WPScore;
class musicTextItem;
class musicBarItem;
class musicRowItem;
class musicWholeItem;

class musicScene : public QGraphicsScene
{
    Q_OBJECT

public:
    musicScene(QWidget *parent = 0);
    void setRect(QRectF rect);
    void display();
    void setScore(WPScore *);
    ~musicScene();
    musicWholeItem *widget;
    
    private slots:
    /*
      void addNumber();
      void deleteNumber();
      void cut();
      void copy();
      void paste();
      */

protected:
      void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private:
    WPScore *music;
    QString address;
    int partNumber;
    QVector< QVector <musicTextItem *> > numbers;
    QVector< QVector<musicBarItem *> > bars;
    QVector<musicRowItem *> rows;
    int rowNumber;
    //musicWholeItem *widget;
    static const int musicHeight[12];
};

#endif // MUSICSCENE_H
