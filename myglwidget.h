#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QGLWidget>
#include <QVector>

class MyGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit MyGLWidget(QWidget *parent = 0);
    ~MyGLWidget();
    void setRange(double _l, double _r, double _b, double _t);
    void addPoint(double x, double y, double _red, double _green, double _blue);
    void addLine(double x1, double y1, double x2, double y2, double _red, double _green, double _blue);
    void clear();
    
signals:
    
public slots:

private:
    QVector<double> px, py, lx1, ly1, lx2, ly2;
    int pcnt;
    int lcnt;
    double l, r, b, t;
    QVector<double> pred, pgreen, pblue, lred, lgreen, lblue;
    
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

};

#endif // MYGLWIDGET_H
