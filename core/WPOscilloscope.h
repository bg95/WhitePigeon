#ifndef WPOSCILLOSCOPE_H
#define WPOSCILLOSCOPE_H

#include <QGLWidget>
#include <QIODevice>
#include <QTimer>
#include "include/core/WPWave.h"

class WPOscilloscope : public QGLWidget
{
    Q_OBJECT
public:
    explicit WPOscilloscope(QWidget *parent = 0);
    ~WPOscilloscope();

    void setInputDevice(QIODevice &_inputdevice);
    void start(quint32 _period, quint32 _length);

public slots:
    void refresh();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

private:
    QTimer *timer;
    QIODevice *inputdevice;
    WPWave wave;
    quint32 period, length;

};

#endif // WPOSCILLOSCOPE_H
