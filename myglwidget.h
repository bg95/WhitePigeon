#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QGLWidget>

class MyGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    static const int N = 2 * 48000;
    explicit MyGLWidget(QWidget *parent = 0);
    ~MyGLWidget();
    void setRange(double _l, double _r, double _b, double _t);
    void addPoint(double x, double y);
    void addLine(double x1, double y1, double x2, double y2);
    void clear();
    
signals:
    
public slots:

private:
    double *px, *py;
    int pcnt;
    double *lx1, *ly1, *lx2, *ly2;
    int lcnt;
    double l, r, b, t;
    
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

};

#endif // MYGLWIDGET_H
