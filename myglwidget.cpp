#include "myglwidget.h"
#include <stdio.h>

MyGLWidget::MyGLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    px = new double[N];
    py = new double[N];
    lx1 = new double[N];
    ly1 = new double[N];
    lx2 = new double[N];
    ly2 = new double[N];
    clear();
}

MyGLWidget::~MyGLWidget()
{
    delete[] px;
    delete[] py;
    delete[] lx1;
    delete[] ly1;
    delete[] lx2;
    delete[] ly2;
}

void MyGLWidget::addPoint(double x, double y)
{
    px[pcnt] = x;
    py[pcnt] = y;
    pcnt++;
    //printf("point %.2lf,%.2lf added\n", x, y);
    //fflush(stdout);
    //paintGL();
}

void MyGLWidget::addLine(double x1, double y1, double x2, double y2)
{
    lx1[lcnt] = x1;
    ly1[lcnt] = y1;
    lx2[lcnt] = x2;
    ly2[lcnt] = y2;
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
    printf("%lf %lf %lf %lf\n", l, r, b, t);
    //glOrtho(-100, 100, -100, 100, -1, 1);
    glColor3d(1.0, 1.0, 0.0);
    glBegin(GL_LINES);
    for (i = 0; i < lcnt; i++)
    {
        glVertex2d(lx1[i], ly1[i]);
        glVertex2d(lx2[i], ly2[i]);
    }
    glEnd();
    glColor3d(0.0, 0.0, 1.0);
    glBegin(GL_LINE_STRIP);
    for (i = 0; i < pcnt; i++)
        glVertex2d(px[i], py[i]);
    glEnd();
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
