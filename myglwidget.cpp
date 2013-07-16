#include "myglwidget.h"
#include <stdio.h>

MyGLWidget::MyGLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    clear();
}

MyGLWidget::~MyGLWidget()
{
}

void MyGLWidget::addPoint(double x, double y, double _red, double _green, double _blue)
{
    px.push_back(x);
    py.push_back(y);
    pred.push_back(_red);
    pgreen.push_back(_green);
    pblue.push_back(_blue);
    pcnt++;
    //printf("point %.2lf,%.2lf added\n", x, y);
    //fflush(stdout);
    //paintGL();
}

void MyGLWidget::addLine(double x1, double y1, double x2, double y2, double _red, double _green, double _blue)
{
    lx1.push_back(x1);
    ly1.push_back(y1);
    lx2.push_back(x2);
    ly2.push_back(y2);
    lred.push_back(_red);
    lgreen.push_back(_green);
    lblue.push_back(_blue);
    lcnt++;
}

void MyGLWidget::setRange(double _l, double _r, double _b, double _t)
{
    l = _l;
    r = _r;
    b = _b;
    t = _t;
    glOrtho(l, r, b, t, 0, 1);
}

void MyGLWidget::clear()
{
    pcnt = 0;
    lcnt = 0;
    px.clear();
    py.clear();
    lx1.clear();
    ly1.clear();
    lx2.clear();
    ly2.clear();
    pred.clear();
    pgreen.clear();
    pblue.clear();
    lred.clear();
    lgreen.clear();
    lblue.clear();
    paintGL();
}

void MyGLWidget::initializeGL()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
}

void MyGLWidget::paintGL()
{
    int i;
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glOrtho(l, r, b, t, -1, 1);
    for (i = 0; i < lcnt; i++)
    {
        glBegin(GL_LINES);
        glColor3d(lred[i], lgreen[i], lblue[i]);
        glVertex2d(lx1[i], ly1[i]);
        glVertex2d(lx2[i], ly2[i]);
        glEnd();
    }
    for (i = 0; i < pcnt; i++)
    {
        glColor3d(pred[i], pgreen[i], pblue[i]);
        glBegin(GL_POINTS);
        glVertex2d(px[i], py[i]);
        glEnd();
    }
}

void MyGLWidget::resizeGL(int w, int h)
{
    //glOrtho(l, r, b, t, -1, 1);
    if(0 == h)
        h = 1;//防止一条边为0
    glViewport(0, 0, (GLint)w, (GLint)h);//重置当前视口，本身不是重置窗口的，只不过是这里被Qt给封装好了
    glMatrixMode(GL_PROJECTION);//选择投影矩阵
    glLoadIdentity();//重置选择好的投影矩阵
    //gluPerspective(45.0, (GLfloat)width/(GLfloat)height, 0.1, 100.0);//建立透视投影矩阵
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    paintGL();
}
