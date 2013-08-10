#ifndef OSCILLOSCOPEWINDOW_H
#define OSCILLOSCOPEWINDOW_H

#include <QMainWindow>
#include <QGLWidget>
#include <QAudioInput>
#include "core/WPWave.h"
#include "core/WPOscilloscope.h"
#include "core/WPPipe.h"
#include "WPScore/WPScore.h"
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
    WPPipe *pipe;
    WPScore *score;

protected:
    void showEvent(QShowEvent *);
    void resizeEvent(QResizeEvent *);
    void hideEvent(QHideEvent *);

};

#endif // OSCILLOSCOPEWINDOW_H
