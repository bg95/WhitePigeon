#ifndef MUSICLINEITEM_H
#define MUSICLINEITEM_H

#include <QGraphicsLineItem>
//#include <QList>

class musicTextItem;

class musicLineItem : public QGraphicsLineItem
{

public:
    musicLineItem();
    ~musicLineItem();
    void setColor(const QColor &color);
    //trackNodes();

private:
    //musicTextItem *leftparent;
    //musicTextItem *rightparent;
};

#endif // MUSICLINEITEM_H
