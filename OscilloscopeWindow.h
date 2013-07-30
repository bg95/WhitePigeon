#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGLWidget>
#include <QAudioInput>
#include "core/WPWave.h"
#include "core/WPOscilloscope.h"
#include "myglwidget.h"

namespace Ui {
class OscilloscopeWindow;
}

class OscilloscopeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit OscilloscopeWindow(QWidget *parent = 0);
    ~OscilloscopeWindow();

public slots:
    void waveDecodeFinished();

private:
    Ui::OscilloscopeWindow *ui;
    //MyGLWidget glwidget, glwidgetR, glwidgetI, glwidgetSTFT;
    WPOscilloscope oscilloscope;
    WPWave wave;
    QAudioInput *audioinput;

protected:
    void resizeEvent(QResizeEvent *);

};

#endif // MAINWINDOW_H
