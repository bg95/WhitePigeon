#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGLWidget>
#include <QAudioInput>
#include "core/WPWave.h"
#include "core/WPOscilloscope.h"
#include "myglwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void waveDecodeFinished();

private:
    Ui::MainWindow *ui;
    //MyGLWidget glwidget, glwidgetR, glwidgetI, glwidgetSTFT;
    WPOscilloscope oscilloscope;
    WPWave wave;
    QAudioInput *audioinput;

protected:
    void resizeEvent(QResizeEvent *);

};

#endif // MAINWINDOW_H
