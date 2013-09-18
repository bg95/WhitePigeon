#ifndef MUSICVIEW_H
#define MUSICVIEW_H

#include <QGraphicsView>

class musicView : public QGraphicsView {
  Q_OBJECT
 public:
  musicView(QGraphicsScene *scene, QWidget *parent = 0);
  void setScene(QGraphicsScene *scene);
  musicView(QWidget *parent = 0);
 protected:
  void wheelEvent(QWheelEvent *event);
};

#endif 
